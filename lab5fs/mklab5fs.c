/*
 * mklab5fs.c
 *
 *  Created on: Nov 27, 2018
 *      Author: haseeb
 */

#include <linux/fs.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "lab5fs.h"

int main(int argc, char *argv[]) {
    char *c = "This is a test\0";
    int len = strlen(c);
    char *fn = NULL;
    FILE *file = NULL;
    struct  lab5fs_sb sb;
    struct lab5fs_inode root_inode;


    fn = argv[1];
    file = fopen(fn, "r+");

    /* Write superblock */
    sb.s_magic = LAB5FS_MAGIC;
    sb.s_blocksize = LAB5FS_BLOCKSIZE;
    sb.s_blocksize_bits = 9;

    sb.s_inode_blocks_total = 1250;
    sb.s_inode_blocks_free  = sb.s_inode_blocks_total - 1;

        struct timespec ctime;
        clock_gettime(CLOCK_REALTIME, &ctime);
        root_inode.i_uid  = 0;
        root_inode.i_gid  = 0;

        root_inode.i_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;

        root_inode.i_atime = ctime;
        root_inode.i_mtime = ctime;
        root_inode.i_ctime = ctime;
        root_inode.i_sblock = 0;
        root_inode.i_eblock = 0;
        root_inode.i_nlink = 1;
        root_inode.i_num_blocks = 0;
        root_inode.i_size = 2;
//        memset(root_inode.name, '\0', LAB5FS_NAMELEN);
        strcpy(root_inode.name, "/");


    fwrite(&sb, 1, sizeof(struct lab5fs_sb), file);

    /* Inode bitmap */
    fseek(file, BLOCK_N(1, sb.s_blocksize), SEEK_SET);
    fprintf(file, "%c", 1);

    /* Data bitmap */
    fseek(file, BLOCK_N(2, sb.s_blocksize), SEEK_SET);
    fprintf(file, "%c%c%c%c", 0, 1, 1, 1);

    /* Root inode */
    fseek(file, INODE_N(0, sb.s_blocksize), SEEK_SET);
    fwrite(&root_inode, 1, sizeof(struct lab5fs_inode), file);

    /* Close out file */
    //fseek(file, 10240, SEEK_SET);
    //fprintf(file, "%c", 0);
    fclose(file);
    
	printf("Properly mounted in image\n");
	return 0;
}
