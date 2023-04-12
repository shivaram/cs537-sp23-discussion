#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <stdbool.h>

#define stat xv6_stat  // avoid clash with host struct stat
#define dirent xv6_dirent  // avoid clash with host struct stat
#include "types.h"
#include "fs.h"
#include "stat.h"
#undef stat
#undef dirent


void print_inode(struct dinode dip);

int main(int argc, char *argv[])
{
    int fd;
    
    //Usage: <my_prog> <fs_img>
    if(argc == 2)
    {
        //if the image file exits, open it in read only mode
        fd = open(argv[1], O_RDONLY);
    }
    else
    {
        printf("Usage: program fs.img\n");
        exit(1);
    }

    if(fd < 0)
    {
        printf("Usage: %s file not found\n", argv[1]);
        exit(1);
    }

    //File opens correctly

    struct stat sbuf;
    fstat(fd, &sbuf);
    printf("Image that i read is %ld in size\n", sbuf.st_size);

    //if you want to access file's contents, you can do two things
    // (a) user read/write/fread/fwrite API -- stream orieted methods, tricky to fo back and forth
    // (b) use mmap


    //maps a file's contents into virtual memory
    /*
     --------
     FILE CONTENTS
     ----------
     heap
     ----------
     stack
     ----------
    */

    //Once this call completes, img_ptr has contents of fd
    void *img_ptr = mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);


    // Block 0 is unused.
    // Block 1 is super block.
    // Inodes start at block 2.
    //It is interpreting bytes from bytes 512 as struct superblock

   struct superblock *sb = (struct superblock *) (img_ptr + BSIZE);
   printf("size %d nblocks %d ninodes %d\n", sb->size, sb->nblocks, sb->ninodes);
    

    // inode 0 i unused
    // inode 1 belongs to the root dir
    // inode has 12 direct data blocks, and 1 indirect data blocks

    //unused | superblock | inodes blocks | 
    // root inode is probably in block 2

    struct dinode *dip = (struct dinode *) (img_ptr + 2 * BSIZE);
    //inode 0 is unused
    print_inode(dip[0]);
    //inode 1 is the root dir and has 512 bytes, data is at block 29
    print_inode(dip[1]);


    //How can we access the content of root dir?
    //dip[1] is the root inode and addrs[0] is the first block within that
    struct xv6_dirent *dent = (struct xv6_dirent *) (img_ptr + dip[1].addrs[0] * BSIZE);
    //loop through the dir entries
    for(int i = 0; i < 5; i++)
    {
    printf("dir ent %d has name %s and inum %d\n", i, dent->name, dent->inum);
    dent++;
    }

   printf("Number of inodes in each block %ld\n", IPB);
}

void print_inode(struct dinode dip)
{
    printf("file type: %d\n", dip.type);
    printf("nlink: %d\n", dip.nlink);
    printf("size: %d\n", dip.size);
    printf("first addrs: %d\n", dip.addrs[0]);
    printf("second addrs: %d\n", dip.addrs[1]);

}



