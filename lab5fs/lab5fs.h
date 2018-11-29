/*
 * lab5s.h
 *
 *  Created on: Nov 27, 2018
 *      Author: haseeb
 */

#ifndef LAB5FS_P_LAB5S_H_
#define LAB5FS_P_LAB5S_H_
#pragma once

/* some random number */
#define LAB5FS_MAGIC		0x856958f6
#define LAB5FS_BLOCKSIZE	512

#define INODE_BITMAP_BLOCK			1
#define DATA_BITMAP_BLOCK			2
//    sb.inode_bitmap_loc = BLOCK_N(1, sb.blocksize);
//    sb.data_bitmap_loc = BLOCK_N(2, sb.blocksize);
/* Macros */
#define BYTES_TO_BLOCKS(loc, blocksize) (loc/blocksize)
#define BLOCK_N(n, blocksize)           (n*blocksize)
#define INODE_N(n, bs)                  (BLOCK_N(3,bs)+(n*bs))
#define DATA_N(n, bs)                   (BLOCK_N(129,bs)+(n*bs))

/* BFS inode layout on disk */
struct lab5fs_inode {
	unsigned short i_ino;
	unsigned short i_unused;
	unsigned int i_sblock;
	unsigned int i_eblock;
	unsigned int i_num_blocks;
	unsigned int i_size;
	unsigned int i_eoffset;
	unsigned int i_vtype;
	unsigned int i_mode;
	__signed__ int i_uid;
	__signed__ int i_gid;
	unsigned int i_nlink;
	unsigned int i_atime;
	unsigned int i_mtime;
	unsigned int i_ctime;
	unsigned int i_padding[4];
};

#define BFS_NAMELEN		14
#define BFS_DIRENT_SIZE		16
#define BFS_DIRS_PER_BLOCK	32

struct bfs_dirent {
	unsigned short ino;
	char name[BFS_NAMELEN];
};

/* BFS superblock layout on disk */
struct lab5fs_sb {
	unsigned int s_magic;
	unsigned int s_blocksize;
	unsigned int s_blocksize_bits;
	unsigned int s_start;
	unsigned int s_end;
	__signed__ int s_from;
	__signed__ int s_to;
	__signed__ int s_bfrom;
	__signed__ int s_bto;
	char  s_fsname[6];
	char  s_volume[6];
	unsigned int s_padding[118];
};


#endif /* LAB5FS_P_LAB5S_H_ */
