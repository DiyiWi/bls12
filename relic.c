#include "relic_bench.c"
#include "relic_conf.c"
#include "relic_core.c"
#include "relic_err.c"
#include "relic_pool.c"
#include "relic_test.c"
#include "relic_trace.c"
#include "relic_util.c"

#if ARCH==ARM
#include "arch/relic_arch_arm.c"
#elif ARCH==X64
#include "arch/relic_arch_x64.c"
#elif ARCH==X86
#include "arch/relic_arch_x86.c"
#endif

#include "bn/relic_bn_add.c"
#include "bn/relic_bn_cmp.c"
#include "bn/relic_bn_div.c"
#include "bn/relic_bn_factor.c"
#include "bn/relic_bn_gcd.c"
#include "bn/relic_bn_lcm.c"
#include "bn/relic_bn_mem.c"
#include "bn/relic_bn_mod.c"
#include "bn/relic_bn_mul.c"
#include "bn/relic_bn_mxp.c"
#include "bn/relic_bn_prime.c"
#include "bn/relic_bn_rec.c"
#include "bn/relic_bn_shift.c"
#include "bn/relic_bn_smb.c"
#include "bn/relic_bn_sqr.c"
#include "bn/relic_bn_srt.c"
#include "bn/relic_bn_util.c"
#if 0
#include "cp/relic_cp_bbs.c"
#include "cp/relic_cp_bdpe.c"
#include "cp/relic_cp_bgn.c"
#include "cp/relic_cp_bls.c"
#include "cp/relic_cp_ecdh.c"
#include "cp/relic_cp_ecdsa.c"
#include "cp/relic_cp_ecies.c"
#include "cp/relic_cp_ecmqv.c"
#include "cp/relic_cp_ecss.c"
#include "cp/relic_cp_ibe.c"
#include "cp/relic_cp_phpe.c"
#include "cp/relic_cp_rabin.c"
#include "cp/relic_cp_rsa.c"
#include "cp/relic_cp_sokaka.c"
#include "cp/relic_cp_vbnn_ibs.c"
#include "cp/relic_cp_zss.c"
#endif

#include "dv/relic_dv_mem.c"
#include "dv/relic_dv_util.c"

#if 0
#include "eb/relic_eb_add.c"
#include "eb/relic_eb_curve.c"
#include "eb/relic_eb_dbl.c"
#include "eb/relic_eb_frb.c"
#include "eb/relic_eb_hlv.c"
#include "eb/relic_eb_map.c"
#include "eb/relic_eb_mul.c"
#include "eb/relic_eb_mul_fix.c"
#include "eb/relic_eb_mul_sim.c"
#include "eb/relic_eb_neg.c"
#include "eb/relic_eb_norm.c"
#include "eb/relic_eb_param.c"
#include "eb/relic_eb_pck.c"
#include "eb/relic_eb_util.c"
#include "ed/relic_ed_add.c"
#include "ed/relic_ed_curve.c"
#include "ed/relic_ed_dbl.c"
#include "ed/relic_ed_map.c"
#include "ed/relic_ed_mul.c"
#include "ed/relic_ed_mul_fix.c"
#include "ed/relic_ed_mul_sim.c"
#include "ed/relic_ed_param.c"
#include "ed/relic_ed_pck.c"
#include "ed/relic_ed_util.c"
#endif

#include "ep/relic_ep_add.c"
#include "ep/relic_ep_curve.c"
#include "ep/relic_ep_dbl.c"
#include "ep/relic_ep_map.c"
#include "ep/relic_ep_mul.c"
#include "ep/relic_ep_mul_fix.c"
#include "ep/relic_ep_mul_sim.c"
#include "ep/relic_ep_neg.c"
#include "ep/relic_ep_norm.c"
#include "ep/relic_ep_param.c"
#undef ASSIGN
#include "ep/relic_ep_pck.c"
#include "ep/relic_ep_util.c"
#include "epx/relic_ep2_add.c"
#include "epx/relic_ep2_curve.c"
#include "epx/relic_ep2_dbl.c"
#include "epx/relic_ep2_frb.c"
#include "epx/relic_ep2_map.c"
#include "epx/relic_ep2_mul.c"
#include "epx/relic_ep2_mul_fix.c"
#include "epx/relic_ep2_mul_sim.c"
#include "epx/relic_ep2_neg.c"
#include "epx/relic_ep2_norm.c"
#include "epx/relic_ep2_pck.c"
#include "epx/relic_ep2_util.c"

#if 0
#include "fb/relic_fb_add.c"
#include "fb/relic_fb_cmp.c"
#include "fb/relic_fb_exp.c"
#include "fb/relic_fb_inv.c"
#include "fb/relic_fb_itr.c"
#include "fb/relic_fb_mul.c"
#include "fb/relic_fb_param.c"
#include "fb/relic_fb_poly.c"
#include "fb/relic_fb_rdc.c"
#include "fb/relic_fb_shift.c"
#include "fb/relic_fb_slv.c"
#include "fb/relic_fb_sqr.c"
#include "fb/relic_fb_srt.c"
#include "fb/relic_fb_trc.c"
#include "fb/relic_fb_util.c"
#include "fbx/relic_fb2_inv.c"
#include "fbx/relic_fb2_mul.c"
#include "fbx/relic_fb2_slv.c"
#include "fbx/relic_fb2_sqr.c"
#endif

#include "fp/relic_fp_add.c"
#include "fp/relic_fp_cmp.c"
#include "fp/relic_fp_exp.c"
#include "fp/relic_fp_inv.c"
#include "fp/relic_fp_mul.c"
#include "fp/relic_fp_param.c"
#include "fp/relic_fp_prime.c"
#include "fp/relic_fp_rdc.c"
#include "fp/relic_fp_shift.c"
#include "fp/relic_fp_sqr.c"
#include "fp/relic_fp_srt.c"
#include "fp/relic_fp_util.c"
#include "fpx/relic_fp12_mul.c"
#include "fpx/relic_fp12_sqr.c"
#include "fpx/relic_fp18_mul.c"
#include "fpx/relic_fp18_sqr.c"
#include "fpx/relic_fp2_mul.c"
#include "fpx/relic_fp2_sqr.c"
#include "fpx/relic_fp3_mul.c"
#include "fpx/relic_fp3_sqr.c"
#include "fpx/relic_fp6_mul.c"
#include "fpx/relic_fp6_sqr.c"
#include "fpx/relic_fpx_add.c"
#include "fpx/relic_fpx_cmp.c"
#include "fpx/relic_fpx_exp.c"
#include "fpx/relic_fpx_frb.c"
#include "fpx/relic_fpx_inv.c"
#include "fpx/relic_fpx_pck.c"
#include "fpx/relic_fpx_rdc.c"
#include "fpx/relic_fpx_srt.c"
#include "fpx/relic_fpx_util.c"

#if 0
#include "md/blake2s-ref.c"
#include "md/relic_md_blake2s.c"
#include "md/relic_md_hmac.c"
#include "md/relic_md_kdf.c"
#include "md/relic_md_mgf.c"
#include "md/relic_md_sha1.c"
#include "md/relic_md_sha224.c"
#include "md/relic_md_sha384.c"
#include "md/relic_md_sha512.c"
#include "md/sha1.c"
#include "md/sha384-512.c"
#endif
#include "md/sha224-256.c"
#include "md/relic_md_sha256.c"

#include "pc/relic_pc_util.c"
#include "pp/relic_pp_add.c"
#include "pp/relic_pp_dbl.c"
#include "pp/relic_pp_exp.c"
#include "pp/relic_pp_map.c"
#include "pp/relic_pp_norm.c"

#if 0
#include "rand/relic_rand_call.c"
#include "rand/relic_rand_core.c"
#include "rand/relic_rand_fips.c"
#include "rand/relic_rand_hash.c"
#include "rand/relic_rand_rdrnd.c"
#include "rand/relic_rand_udev.c"
#endif


#include "low/easy/relic_bn_add_low.c"
#include "low/easy/relic_bn_cmp_low.c"
#include "low/easy/relic_bn_div_low.c"
#include "low/easy/relic_bn_mod_low.c"
#include "low/easy/relic_bn_mul_low.c"
#include "low/easy/relic_bn_sqr_low.c"
#include "low/easy/relic_bn_shift_low.c"

//#include "low/easy/relic_fp_add_low.c"
#include "low/easy/relic_fp_cmp_low.c"
#include "low/easy/relic_fp_inv_low.c"
//#include "low/easy/relic_fp_rdc_low.c"
//#include "low/easy/relic_fp_shift_low.c"
#include "low/easy/relic_fp_sqr_low.c"
#include "low/easy/relic_fpx_add_low.c"
#include "low/easy/relic_fpx_mul_low.c"
#include "low/easy/relic_fpx_rdc_low.c"
#include "low/easy/relic_fpx_sqr_low.c"

#define fp_muln_low fp_muln_low_
#define fp_mulm_low fp_mulm_low_
#include "low/easy/relic_fp_mul_low.c"
#define fp_rdcn_low fp_rdcn_low_
#include "low/easy/relic_fp_rdc_low.c"

#define fp_lsh1_low fp_lsh1_low_
#define fp_rsh1_low fp_rsh1_low_
#include "low/easy/relic_fp_shift_low.c"