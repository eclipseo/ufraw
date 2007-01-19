/*
   dcraw.h - Dave Coffin's raw photo decoder - header for C++ adaptation
   Copyright 1997-2007 by Dave Coffin, dcoffin a cybercom o net
   Copyright 2004-2007 by Udi Fuchs, udifuchs a gmail o com
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License version 2
   as published by the Free Software Foundation. You should have received
   a copy of the license along with this program.
 
   This is a adaptation of Dave Coffin's original dcraw.c to C++.
   It can work as either a command-line tool or called by other programs.
 
   Notice that the original dcraw.c is published under a different
   license. Naturaly, the GPL license applies only to this derived
   work.
 */

#define ushort UshORt
typedef unsigned char uchar;
typedef unsigned short ushort;

class DCRaw { public:
/* All dcraw's global variables are members of this class. */
FILE *ifp;
short order;
char *ifname, make[64], model[64], model2[64], *meta_data, cdesc[5];
float flash_used, canon_ev, iso_speed, shutter, aperture, focal_len;
time_t timestamp;
unsigned shot_order, kodak_cbpp, filters, unique_id, *oprof;
unsigned profile_offset, profile_length;
unsigned data_offset, strip_offset, curve_offset, meta_offset, meta_length;
int thumb_offset, thumb_length, thumb_width, thumb_height, thumb_misc;
int tiff_nifds, tiff_flip, tiff_bps, tiff_compress, tile_length;
int raw_height, raw_width, top_margin, left_margin;
int height, width, fuji_width, colors, tiff_samples;
int black, maximum, raw_color, use_gamma;
int iheight, iwidth, shrink, flip;
double pixel_aspect;
int zero_after_ff, is_raw, dng_version, is_foveon;
ushort (*image)[4], white[8][8], curve[0x1000], cr2_slice[3];
float bright, user_mul[4], sigma_d, sigma_r;
int four_color_rgb, document_mode, highlight;
int verbose, use_auto_wb, use_camera_wb;
int output_color, output_bps, output_tiff;
int fuji_layout, fuji_secondary, shot_select;
float cam_mul[4], pre_mul[4], rgb_cam[3][4];	/* RGB from camera color */
int histogram[4][0x2000];
void (DCRaw::*write_thumb)(FILE *), (DCRaw::*write_fun)(FILE *);
void (DCRaw::*load_raw)(), (DCRaw::*thumb_load_raw)();
jmp_buf failure;

struct decode {
  struct decode *branch[2];
  int leaf;
} first_decode[2048], *second_decode, *free_decode;

struct {
  int width, height, bps, comp, phint, offset, flip, samples, bytes;
} tiff_ifd[10];

struct {
  int format, key_off, black, black_off, split_col, tag_21a;
  float tag_210;
} ph1;

int tone_curve_size, tone_curve_offset; /* Nikon Tone Curves UF*/
int tone_mode_offset, tone_mode_size; /* Nikon ToneComp UF*/

/* Used by dcraw_message() */
char *messageBuffer;
int lastStatus;

/* Initialization of the variables is done here */
DCRaw();
void dcraw_message(int code, const char *format, ...);
/* All dcraw functions with the CLASS prefix are members of this class. */
int fc (int row, int col);
void merror (void *ptr, char *where);
ushort sget2 (uchar *s);
ushort get2();
int sget4 (uchar *s);
int get4();
int getint (int type);
float int_to_float (int i);
double getreal (int type);
void read_shorts (ushort *pixel, int count);
void canon_600_fixed_wb (int temp);
int canon_600_color (int ratio[2], int mar);
void canon_600_auto_wb();
void canon_600_coeff();
void canon_600_load_raw();
void remove_zeroes();
void canon_a5_load_raw();
unsigned getbits (int nbits);
void init_decoder();
uchar * make_decoder (const uchar *source, int level);
void crw_init_tables (unsigned table);
int canon_has_lowbits();
void canon_compressed_load_raw();
int ljpeg_start (struct jhead *jh, int info_only);
int ljpeg_diff (struct decode *dindex);
void ljpeg_row (int jrow, struct jhead *jh);
void lossless_jpeg_load_raw();
void adobe_copy_pixel (int row, int col, ushort **rp);
void adobe_dng_load_raw_lj();
void adobe_dng_load_raw_nc();
void pentax_k10_load_raw();
void nikon_compressed_load_raw();
void nikon_load_raw();
int nikon_is_compressed();
int nikon_e995();
int nikon_e2100();
void nikon_3700();
int minolta_z2();
void nikon_e900_load_raw();
void nikon_e2100_load_raw();
void fuji_load_raw();
void jpeg_thumb (FILE *tfp);
void ppm_thumb (FILE *tfp);
void layer_thumb (FILE *tfp);
void rollei_thumb (FILE *tfp);
void rollei_load_raw();
int bayer (unsigned row, unsigned col);
void phase_one_flat_field (int is_float, int nc);
void phase_one_correct();
void phase_one_load_raw();
unsigned ph1_bits (int nbits);
void phase_one_load_raw_c();
void leaf_hdr_load_raw();
void sinar_4shot_load_raw();
void imacon_full_load_raw();
void packed_12_load_raw();
void unpacked_load_raw();
void panasonic_load_raw();
void olympus_e300_load_raw();
void olympus_cseries_load_raw();
void minolta_rd175_load_raw();
void eight_bit_load_raw();
void casio_qv5700_load_raw();
void nucore_load_raw();
const int * make_decoder_int (const int *source, int level);
int radc_token (int tree);
void kodak_radc_load_raw();
#ifndef HAVE_LIBJPEG
void kodak_jpeg_load_raw();
#else
void kodak_jpeg_load_raw();
#endif
void kodak_dc120_load_raw();
void kodak_easy_load_raw();
void kodak_262_load_raw();
int kodak_65000_decode (short *out, int bsize);
void kodak_65000_load_raw();
void kodak_ycbcr_load_raw();
void kodak_rgb_load_raw();
void kodak_thumb_load_raw();
void sony_decrypt (unsigned *data, int len, int start, int key);
void sony_load_raw();
void sony_arw_load_raw();
void smal_decode_segment (unsigned seg[2][2], int holes);
void smal_v6_load_raw();
int median4 (int *p);
void fill_holes (int holes);
void smal_v9_load_raw();
void foveon_decoder (unsigned size, unsigned code);
void foveon_thumb (FILE *tfp);
void foveon_load_camf();
void foveon_load_raw();
const char * foveon_camf_param (const char *block, const char *param);
void * foveon_camf_matrix (int dim[3], const char *name);
int foveon_fixed (void *ptr, int size, const char *name);
float foveon_avg (short *pix, int range[2], float cfilt);
short * foveon_make_curve (double max, double mul, double filt);
void foveon_make_curves
	(short **curvep, float dq[3], float div[3], float filt);
int foveon_apply_curve (short *curve, int i);
void foveon_interpolate();
void bad_pixels();
void subtract(char *fname);
void pseudoinverse (double (*in)[3], double (*out)[3], int size);
void cam_xyz_coeff (double cam_xyz[4][3]);
void colorcheck();
void scale_colors();
void border_interpolate (int border);
void lin_interpolate();
void vng_interpolate();
void cam_to_cielab (ushort cam[4], float lab[3]);
void ahd_interpolate();
void bilateral_filter();
void recover_highlights();
void tiff_get (unsigned base,
	unsigned *tag, unsigned *type, unsigned *len, unsigned *save);
void parse_thumb_note (int base, unsigned toff, unsigned tlen);
void parse_makernote (int base);
void get_timestamp (int reversed);
void parse_exif (int base);
void romm_coeff (float romm_cam[3][3]);
void parse_mos (int offset);
void linear_table (unsigned len);
int parse_tiff_ifd (int base, int level);
void parse_kodak_ifd (int base);
void parse_tiff (int base);
void parse_minolta (int base);
void parse_external_jpeg();
void ciff_block_1030();
void parse_ciff (int offset, int length);
void parse_rollei();
void parse_sinar_ia();
void parse_phase_one (int base);
void parse_fuji (int offset);
int parse_jpeg (int offset);
void parse_riff();
void parse_smal (int offset, int fsize);
char * foveon_gets (int offset, char *str, int len);
void parse_foveon();
void adobe_coeff (char *make, char *model);
void simple_coeff (int index);
short guess_byte_order (int words);
void identify();
void apply_profile (char *input, char *output);
void convert_to_rgb();
void fuji_rotate();
void stretch();
int flip_index (int row, int col);
void gamma_lut (uchar lut[0x10000]);
void tiff_set (ushort *ntag,
	        ushort tag, ushort type, int count, int val);
void tiff_head (struct tiff_hdr *th, int full);
void write_ppm_tiff (FILE *ofp);
void write_ppm (FILE *ofp);
void write_ppm16 (FILE *ofp);
void write_psd (FILE *ofp);
int main (int argc, char **argv);
};
