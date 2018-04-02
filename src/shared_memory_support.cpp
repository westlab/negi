
#include "shared_memory_support.h"


void *set_up_mmap(const char * file_name, int file_size, int options, int mode){
	FILE * file_fp = fopen (file_name, "w");
	fseek(file_fp, file_size , SEEK_SET);
	fputc('\0', file_fp);
	fclose(file_fp);
	
	return (void *)set_up_mmap_slave(file_name, options, mode);
}

void *set_up_mmap_slave(const char * file_name, int options, int mode){
	struct stat sb;
	int file = open (file_name, O_RDWR);

	if (file == -1) {
		perror ("file open failed");
		exit(1);
	}

	if (fstat (file, &sb) == -1) {
		perror ("fstat failed");
		exit(1);
	}
	if (!S_ISREG (sb.st_mode)) {
		fprintf (stderr, "%s is not a file\n", file_name);
		exit(1);
	}
	return (void *)mmap ( 0, sb.st_size, options, mode, file, 0);
}

void *shm_setup(key_t key, int *shmid, int shm_size, int mode){

    void *addr;    
    /* connect to (and possibly create) the segment: */
    if (( (*shmid) = shmget(key, shm_size, mode | IPC_CREAT)) == -1) {
        perror("shmget error");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    addr = shmat((*shmid), (void *)0, 0);
    if (addr == (char *)(-1)) {
        perror("shmat error");
        exit(1);
    }
    return addr;
}


int delete_shm(void * addr){
	if (shmdt(addr) == -1) {
        perror("shmdt");
        exit(1);
    }
    return 0;
}

int shm_user_control(int shmid, int uid, int gid, int mode){
	struct shmid_ds shmid_dss;
	struct ipc_perm perms;
	perms.uid = uid;
	perms.gid = gid;
	perms.mode = mode; // Allow read/write only by 
	                   // uid '100' or members of group '200'
	shmid_dss.shm_perm = perms;
	shmctl(shmid, IPC_SET, &shmid_dss);
	return 0;
}

