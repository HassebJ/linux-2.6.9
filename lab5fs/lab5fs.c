/*
 * lab5fs.c
 *
 *  Created on: Nov 4, 2018
 *      Author: haseeb
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/buffer_head.h>
#include <linux/pagemap.h>
#include <asm/uaccess.h>
#include "lab5fs.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("MHJ");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");
#define DEVICE_NAME "lkm_example"
#define EXAMPLE_MSG "Hello, World!\n"
#define MSG_BUFFER_LEN 15

/* Prototypes for device functions */
void lab5fs_read_inode(struct inode *inode);
struct inode *lab5fs_get_inode(struct super_block *sb, int mode, dev_t dev);
static int major_num;
static int device_open_count = 0;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;
static struct inode * root_inode;

inline unsigned long ino_num_to_blk_num(ino_t ino_num) {
  return (BLOCK_N(INODE_BITMAP_BLOCK, LAB5FS_BLOCKSIZE) / LAB5FS_BLOCKSIZE) + ino_num;
}

static int
lab5fs_mknod(struct inode *dir, struct dentry *dentry, int mode, dev_t dev)
{
	struct inode * inode = lab5fs_get_inode(dir->i_sb, mode, dev);
	int error = -ENOSPC;

	if (inode) {
		if (dir->i_mode & S_ISGID) {
			inode->i_gid = dir->i_gid;
			if (S_ISDIR(mode))
				inode->i_mode |= S_ISGID;
		}
		d_instantiate(dentry, inode);
		dget(dentry);	/* Extra count - pin the dentry in core */
		error = 0;
	}
	return error;
}

int lab5fs_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
        struct dentry *dentry = filp->f_dentry;
        struct inode *dir = filp->f_dentry->d_inode;
        struct super_block* sb = dir->i_sb;
        int need_revalidation = (filp->f_version != dir->i_version);
		struct buffer_head *bh = NULL;
		int data_block_num = 0;
		struct stamfs_dir_rec *dir_rec;
		int err = 0;
		int over;

        printk(KERN_INFO "lab5fsfs: readdir, file=%s, pos=%llu, dir->i_size:pos=%llu\n",
                             dentry->d_name.name, filp->f_pos, dir->i_size);

        if (filp->f_pos > dir->i_size) {
        	printk(KERN_INFO "lab5fs: file pos larger then dir size.\n");
                 goto done;
         }

        /* TODO - what does this revalidation, and version information,
         * mean at all? */
        if (need_revalidation) {
                /* TODO - make sure 'pos' points to the beginning of a dir rec. */
                need_revalidation = 0;
        }


        /* special handling for '.' and '..' */
        if (filp->f_pos == 0) {
        	printk(KERN_INFO
                           "lab5fs: readdir, f_pos == 0, adding '.', ino=%lu\n",
                           dir->i_ino);
                over = filldir(dirent, ".", 1, filp->f_pos,
//                               dir->i_ino, DT_DIR);
                			   19, DT_DIR);
                if (over < 0)
                        goto done;
                filp->f_pos++;
        }
        if (filp->f_pos == 1) {
        	printk(KERN_INFO  "lab5fs: readdir, f_pos == 1, "
                                     "adding '..', ino=%lu\n",
                                     dentry->d_parent->d_inode->i_ino);
                over = filldir(dirent, "..", 2, filp->f_pos,
//                               dentry->d_parent->d_inode->i_ino, DT_DIR);
                			   20, DT_DIR);
                if (over < 0)
                        goto done;
                filp->f_pos++;
        }
    	struct dentry *subdir;
    	unsigned char d_type = DT_UNKNOWN;
    	struct list_head *i;
    	list_for_each(i, &dentry->d_subdirs) {
    		struct dentry *subdir = list_entry(i, struct dentry, d_child);

    		switch (subdir->d_inode->i_mode & S_IFMT) {
    		case S_IFREG:
    			d_type = DT_REG;
    			break;
    		case S_IFDIR:
    			d_type = DT_DIR;
    			break;
    		case S_IFLNK:
    			break;
    		default:
    			printk(KERN_INFO "UNKOWN INODE TYPE");
    			break;
    		}
    		printk(KERN_INFO  "lab5fs: readdir, f_pos == %lld, "
                            "adding '%s', ino=%u\n",
                            filp->f_pos, subdir->d_iname,
							subdir->d_inode->i_ino);
    		over = filldir(dirent, subdir->d_iname,DNAME_INLINE_LEN_MIN, filp->f_pos, subdir->d_inode->i_ino, d_type);
    		if (over < 0)
				goto done;
    		filp->f_pos++;
    	    // do something with obj
    	}

  done:
        filp->f_version = dir->i_version;
        if (bh)
                brelse(bh);
//        filp->f_pos = 0;
        return err;
}
//
//struct dentry *lab5fs_lookup(struct inode *dir, struct dentry *dentry)
//{
//        int err = 0;
//        struct inode* ino = NULL;
//        struct dentry *dir_dentry;
//        ino_t ino_num = 0;
//
//    	struct list_head *tmp, *head = &dir->i_dentry;
//    	tmp = head;
//    	while ((tmp = tmp->next) != head) {
//    		dir_dentry = list_entry(tmp, struct dentry, d_child);
//
//    	}
//
//	printk(KERN_INFO "Searching for: %s\n", dentry->d_name.name);
//        struct list_head *i;
//        struct dentry *subdir;
//    	list_for_each(i, &dir_dentry->d_subdirs) {
//    		subdir = list_entry(i, struct dentry, d_child);
//    		printk(KERN_INFO "Current subDirDentry : %s\n", subdir->d_name.name);
//    		if(strcmp(subdir->d_name.name, dentry->d_name.name)){
//    			ino_num = subdir->d_inode->i_ino;
////    			printk(KERN_INFO "Dentry found: %s\n", subdir->d_name.name);
//    			break;
//    		}
//
//    	}
//        if (err == 0 && ino_num > 0) {
//                ino = iget(dir->i_sb, ino_num);
//                if (!ino) {
//                        printk(KERN_INFO "lab5fs: iget failed.\n");
//                        return ERR_PTR(-EACCES);
//                }
//        }
//        else {
//        	printk(KERN_INFO "lab5fs: file not found.\n");
//        }
//
//        d_add(dentry, ino);
//
//        printk(KERN_INFO "lab5fs: after d_add, dentry->d_count==%d\n",
//                             atomic_read(&dentry->d_count));
//
//        return (err == 0 ? NULL : ERR_PTR(err));
//}

static int simple_delete_dentry(struct dentry *dentry)
{
	return 1;
}


struct dentry *lab5fs_lookup(struct inode *dir, struct dentry *dentry, struct nameidata *nd)
{
	static struct dentry_operations simple_dentry_operations = {
		.d_delete = simple_delete_dentry,
	};

	if (dentry->d_name.len > NAME_MAX)
		return ERR_PTR(-ENAMETOOLONG);
	dentry->d_op = &simple_dentry_operations;
	d_add(dentry, NULL);
	return NULL;
}

static int lab5fs_mkdir(struct inode * dir, struct dentry * dentry, int mode)
{
	int retval = lab5fs_mknod(dir, dentry, mode | S_IFDIR, 0);
	if (!retval)
		dir->i_nlink++;
	return retval;
}

static int lab5_rmdir(struct inode *dir, struct dentry *dentry)
{
	dentry->d_inode->i_nlink--;
	simple_unlink(dir, dentry);
	dir->i_nlink--;
	return 0;
}

static int lab5fs_create(struct inode *dir, struct dentry *dentry, int mode, struct nameidata *nd)
{
	return lab5fs_mknod(dir, dentry, mode | S_IFREG, 0);
}

static int lab5fs_link(struct dentry *old_dentry, struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = old_dentry->d_inode;

	inode->i_ctime = dir->i_ctime = dir->i_mtime = CURRENT_TIME;
	inode->i_nlink++;
	atomic_inc(&inode->i_count);
	dget(dentry);
	d_instantiate(dentry, inode);
	return 0;
}

void lab5fs_put_super (struct super_block *sb)
{
	printk(KERN_INFO "INSIDE lab5fs_put_super\n");
	if(sb){
		printk(KERN_INFO " sb not null");
		if(sb->s_root){
			printk(KERN_INFO " s_root not null");
			if(sb->s_root->d_inode){
				printk(KERN_INFO " d_root not null");
				sb->s_root->d_inode->u.generic_ip = NULL;
				iput(sb->s_root->d_inode);
			}
		}

	}
	if(root_inode){
		printk(KERN_INFO "iput on root_indode called");
		iput(root_inode);
	}


//	sb_  ino->u.generic_ip = NULL;
//        struct stamfs_meta_data *stamfs_meta = STAMFS_META(sb);
//
//        STAMFS_DBG(DEB_STAM,
//                   "stamfs: releasing (umount) superblock, dev='%d:%d'\n",
//                   major(sb->s_dev), minor(sb->s_dev));
//
//        /* free memory used by the stamfs-portion of the super-block. */
//        if (!stamfs_meta)
//                BUG();
//
//        brelse(stamfs_meta->s_sbh);
//        brelse(stamfs_meta->s_iibh);
//        brelse(stamfs_meta->s_flbh);
//        kfree(stamfs_meta);
//
//        MOD_DEC_USE_COUNT;
}


int lab5fs_unlink(struct inode *dir, struct dentry *dentry)
{
	struct inode *inode = dentry->d_inode;

	inode->i_ctime = dir->i_ctime = dir->i_mtime = CURRENT_TIME;
	inode->i_nlink--;
	dput(dentry);
	return 0;
}


static struct address_space_operations lab5fs_aops = {
	.readpage	= simple_readpage,
	.prepare_write	= simple_prepare_write,
	.commit_write	= simple_commit_write
};

struct file_operations lab5fs_file_operations = {
	.read		= generic_file_read,
	.write		= generic_file_write,
	.mmap		= generic_file_mmap,
	.fsync		= file_fsync,
	.readdir	= lab5fs_readdir
};

static struct inode_operations lab5fs_file_inode_operations = {
	.getattr	= simple_getattr,
};

static struct inode_operations lab5fs_dir_inode_operations = {
	.create		= lab5fs_create,
//	.lookup		= lab5fs_lookup,
	.lookup		= simple_lookup,
	.link		= lab5fs_link,
	.unlink		= lab5fs_unlink,
//	.symlink	= lab5fs_symlink,
	.mkdir		= lab5fs_mkdir,
	.rmdir		= lab5_rmdir,
	.mknod		= lab5fs_mknod,
//	.rename		= simple_rename,
};

static struct super_operations lab5fs_ops = {
	.read_inode = lab5fs_read_inode,
	.statfs		= simple_statfs,
	.drop_inode	= generic_delete_inode,
	.put_super = lab5fs_put_super,
};


/* Superblock operations */
void lab5fs_read_inode(struct inode *inode)
{
	printk(KERN_INFO "********Inside lab5fs_read_inode********\n");
	ino_t ino = inode->i_ino;
	unsigned long block_addr = ino_num_to_blk_num(ino);
	struct buffer_head *bh = NULL;
	struct lab5fs_inode *inode_temp = NULL;

	if(!inode)
		printk(KERN_INFO "inode is null\n");
	printk(KERN_INFO "Read inode num -> blk = %lu\n", block_addr);
	printk(KERN_INFO "blocksize = %lu\n", inode->i_sb->s_blocksize);

	/* Read at given block address */
//	if (!g_bdev) { printk(KERN_INFO "BLOCK DEV IS NULL!\n"); }

	bh = sb_bread(inode->i_sb, block_addr);
//	bh = __bread(g_bdev, block_addr, l5sb->blocksize);

	if (!bh) { printk(KERN_INFO "BUFFER HEAD IS NULL!\n"); }
	inode_temp = (struct lab5fs_inode *) bh->b_data;

	if (!inode_temp) { printk(KERN_INFO "INODE TEMP IS NULL!\n"); }
	printk(KERN_INFO "Filling out inode info...\n");
	printk(KERN_INFO "labfs_ino.i_size = %lu\n", inode_temp->i_size);
	inode->i_mode = inode_temp->i_mode;
	inode->i_uid = inode_temp->i_uid;
	inode->i_gid = inode_temp->i_gid;
	struct timespec atime =  {
		.tv_sec = inode_temp->i_atime
	};
	struct timespec mtime =  {
		.tv_sec = inode_temp->i_mtime
	};
	struct timespec ctime =  {
		.tv_sec = inode_temp->i_ctime
	};

	inode->i_atime = atime;
	inode->i_mtime = mtime;
	inode->i_ctime = ctime;
	printk(KERN_INFO "Inode temp has %d blocks\n", inode_temp->i_num_blocks);
	inode->i_blocks = inode_temp->i_num_blocks;
	inode->i_size = inode_temp->i_size;
	inode->i_blksize = inode->i_sb->s_blocksize;
	inode->i_nlink = 1;  // temp
	inode->i_fop = &lab5fs_file_operations;
	inode->i_op = &lab5fs_dir_inode_operations;
	inode->i_mapping->a_ops = &lab5fs_aops;

	/* Set up defaults */
	/*inode->i_mode = S_IRUGO|S_IWUSR;
  inode->i_nlink = 1;
  inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;*/

	if (ino == 0) {
		printk(KERN_INFO "lab5fs reading root\n");
		inode->i_mode |= S_IFDIR;
		inode->i_nlink = 2;
//		inode->i_fop = &lab5fs_dir_operations;
//		inode->i_size = inode->i_sb->s_blocksize;
		inode->i_size = 2;
		inode->i_blocks = 1;
		inode->i_bytes = inode->i_sb->s_blocksize;
	}
	// FIXME:
	brelse(bh);
}


struct inode *lab5fs_get_inode(struct super_block *sb, int mode, dev_t dev)
{
	printk(KERN_INFO "Inside get_inode");
	struct inode * inode = new_inode(sb);

	if (inode) {
		inode->i_mode = mode;
		inode->i_uid = current->fsuid;
		inode->i_gid = current->fsgid;
		inode->i_blksize = LAB5FS_BLOCKSIZE;
		inode->i_blocks = 0;
		inode->i_mapping->a_ops = &lab5fs_aops;
//		inode->i_mapping->backing_dev_info = &lab5fs_backing_dev_info;
		inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
		switch (mode & S_IFMT) {
		default:
			init_special_inode(inode, mode, dev);
			break;
		case S_IFREG:
			inode->i_op = &lab5fs_file_inode_operations;
			inode->i_fop = &lab5fs_file_operations;
			break;
		case S_IFDIR:
			inode->i_op = &lab5fs_dir_inode_operations;
//			inode->i_fop = &simple_dir_operations;
			inode->i_fop = &lab5fs_file_operations;

			/* directory inodes start off with i_nlink == 2 (for "." entry) */
			inode->i_nlink++;
			break;
		case S_IFLNK:
			inode->i_op = &page_symlink_inode_operations;
			break;
		}
	}
	return inode;
}

static int lab5fs_fill_super(struct super_block *sb, void *data, int silent){
	struct inode * inode;
	struct dentry * root;

	struct buffer_head *bh = sb_bread(sb, 0);
	struct lab5fs_sb*  l5sb = (struct lab5fs_sb*) bh->b_data;

	if (l5sb) {
	printk(KERN_INFO "Read lab5fs superblock...\n");
	printk(KERN_INFO "Magic number of device: 0x%lX\n", l5sb->s_magic);
	printk(KERN_INFO "Blocksize of device: %lu\n", l5sb->s_blocksize);
	}

	 sb_set_blocksize(sb, LAB5FS_BLOCKSIZE);

	sb->s_maxbytes = MAX_LFS_FILESIZE;
	sb->s_blocksize = l5sb->s_blocksize;
	sb->s_blocksize_bits = l5sb->s_blocksize_bits;
	sb->s_magic = l5sb->s_magic;
	sb->s_op = &lab5fs_ops;

	inode = iget(sb, 0);
	root_inode = inode;
//	inode = lab5fs_get_inode(sb, S_IFDIR | 0755, 0);
//	inode->i_ino = 0;


	if (!inode)
		return -ENOMEM;

	if (inode) {
	printk(KERN_INFO "Read lab5fs root inode: %lu\n", inode->i_ino);
	printk(KERN_INFO "i_size: %lu\n", inode->i_size);
	printk(KERN_INFO "Write time: %lu\n", inode->i_atime.tv_nsec);
	}

	root = d_alloc_root(inode);
	if (!root) {
		iput(inode);
		return -ENOMEM;
	}
	sb->s_root = root;
	brelse(bh);
	return 0;
}

static struct super_block *lab5fs_get_sb(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	return get_sb_bdev(fs_type, flags, dev_name, data, lab5fs_fill_super);
}

static struct file_system_type lab5fs_fs_type = {
	.owner		= THIS_MODULE,
	.name		= "lab5fs",
	.get_sb		= lab5fs_get_sb,
	.kill_sb	= kill_block_super,
	.fs_flags	= FS_REQUIRES_DEV
};

static int __init lab5fs_init(void) {
	/* Fill buffer with our message */
//	strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
//	/* Set the msg_ptr to the buffer */
//	msg_ptr = msg_buffer;
	/* Try to register filesystem */
//	major_num = register_chrdev(0, "lkm_example", &file_ops);


	int err = register_filesystem(&lab5fs_fs_type);
	if (err) {
		printk(KERN_ALERT "Could not lab5fs\n");
		return err;
	} else {
		printk(KERN_INFO "lab5fs module loaded\n");
		return 0;
	}
}


static void __exit lab5fs_exit(void) {
	/* Remember — we have to clean up after ourselves. Unregister the character device. */
//	if(root_inode){
//		printk(KERN_INFO "iput on root_indode called");
//		iput(root_inode);
//	}
	unregister_filesystem(&lab5fs_fs_type);
	printk(KERN_INFO "lab5fs unloaded\n");
}

/* Register module functions */
module_init(lab5fs_init);
module_exit(lab5fs_exit);
