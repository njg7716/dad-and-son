#include <unistd.h>
#include <sys/syscall.h>

int main(char *argv) {
	syscall(216);
}
