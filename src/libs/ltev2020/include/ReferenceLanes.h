/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "RSI"
 * 	found in "./asn/RSI.asn"
 * 	`asn1c -fskeletons-copy -fnative-types -gen-PER -pdu=auto -no-gen-example -D ./src`
 */

#ifndef	_ReferenceLanes_H_
#define	_ReferenceLanes_H_


#include <asn_application.h>

/* Including external dependencies */
#include <BIT_STRING.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ReferenceLanes {
	ReferenceLanes_reserved	= 0,
	ReferenceLanes_lane1	= 1,
	ReferenceLanes_lane2	= 2,
	ReferenceLanes_lane3	= 3,
	ReferenceLanes_lane4	= 4,
	ReferenceLanes_lane5	= 5,
	ReferenceLanes_lane6	= 6,
	ReferenceLanes_lane7	= 7,
	ReferenceLanes_lane8	= 8,
	ReferenceLanes_lane9	= 9,
	ReferenceLanes_lane10	= 10,
	ReferenceLanes_lane11	= 11,
	ReferenceLanes_lane12	= 12,
	ReferenceLanes_lane13	= 13,
	ReferenceLanes_lane14	= 14,
	ReferenceLanes_lane15	= 15
} e_ReferenceLanes;

/* ReferenceLanes */
typedef BIT_STRING_t	 ReferenceLanes_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ReferenceLanes_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ReferenceLanes;
asn_struct_free_f ReferenceLanes_free;
asn_struct_print_f ReferenceLanes_print;
asn_constr_check_f ReferenceLanes_constraint;
ber_type_decoder_f ReferenceLanes_decode_ber;
der_type_encoder_f ReferenceLanes_encode_der;
xer_type_decoder_f ReferenceLanes_decode_xer;
xer_type_encoder_f ReferenceLanes_encode_xer;
oer_type_decoder_f ReferenceLanes_decode_oer;
oer_type_encoder_f ReferenceLanes_encode_oer;
per_type_decoder_f ReferenceLanes_decode_uper;
per_type_encoder_f ReferenceLanes_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ReferenceLanes_H_ */
#include <asn_internal.h>
