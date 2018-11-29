#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

#undef unix
struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = __stringify(KBUILD_MODNAME),
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0x5876218b, "struct_module" },
	{ 0xe508074e, "unregister_filesystem" },
	{ 0x5a420754, "register_filesystem" },
	{ 0x6d0503c3, "kill_block_super" },
	{ 0x21c27caf, "get_sb_bdev" },
	{ 0x45b716cd, "unlock_new_inode" },
	{ 0xd0c62ea0, "d_alloc_root" },
	{ 0x65ab4700, "iget_locked" },
	{ 0xb7e08ac4, "sb_set_blocksize" },
	{ 0x7d79f035, "page_symlink_inode_operations" },
	{ 0x93f34ae8, "init_special_inode" },
	{ 0x3c144dbf, "new_inode" },
	{ 0x8505658c, "__brelse" },
	{ 0x499ad81e, "__bread" },
	{ 0x72a8085, "simple_statfs" },
	{ 0x5f1c65f7, "generic_delete_inode" },
	{ 0x990307f5, "simple_lookup" },
	{ 0x79f65cbf, "simple_getattr" },
	{ 0xbbc8742, "file_fsync" },
	{ 0xb3a4f5a3, "generic_file_mmap" },
	{ 0x8aafadbf, "generic_file_write" },
	{ 0xcf744cca, "generic_file_read" },
	{ 0x7ba08a88, "simple_commit_write" },
	{ 0xd5fa977a, "simple_prepare_write" },
	{ 0xffde1388, "simple_readpage" },
	{ 0xd3f462e1, "dput" },
	{ 0xec9f4adf, "iput" },
	{ 0x74cc238d, "current_kernel_time" },
	{ 0x49fd19aa, "simple_unlink" },
	{ 0x6942b07c, "d_rehash" },
	{ 0x1b7d4074, "printk" },
	{ 0xa7a9b222, "d_instantiate" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

