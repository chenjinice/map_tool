/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MapLane"
 * 	found in "./asn/MapLane.asn"
 * 	`asn1c -fskeletons-copy -fnative-types -gen-PER -pdu=auto -no-gen-example -D ./src`
 */

#ifndef	_LaneTypeAttributes_H_
#define	_LaneTypeAttributes_H_


#include <asn_application.h>

/* Including external dependencies */
#include "LaneAttributes-Vehicle.h"
#include "LaneAttributes-Crosswalk.h"
#include "LaneAttributes-Bike.h"
#include "LaneAttributes-Sidewalk.h"
#include "LaneAttributes-Barrier.h"
#include "LaneAttributes-Striping.h"
#include "LaneAttributes-TrackedVehicle.h"
#include "LaneAttributes-Parking.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum LaneTypeAttributes_PR {
	LaneTypeAttributes_PR_NOTHING,	/* No components present */
	LaneTypeAttributes_PR_vehicle,
	LaneTypeAttributes_PR_crosswalk,
	LaneTypeAttributes_PR_bikeLane,
	LaneTypeAttributes_PR_sidewalk,
	LaneTypeAttributes_PR_median,
	LaneTypeAttributes_PR_striping,
	LaneTypeAttributes_PR_trackedVehicle,
	LaneTypeAttributes_PR_parking
	/* Extensions may appear below */
	
} LaneTypeAttributes_PR;

/* LaneTypeAttributes */
typedef struct LaneTypeAttributes {
	LaneTypeAttributes_PR present;
	union LaneTypeAttributes_u {
		LaneAttributes_Vehicle_t	 vehicle;
		LaneAttributes_Crosswalk_t	 crosswalk;
		LaneAttributes_Bike_t	 bikeLane;
		LaneAttributes_Sidewalk_t	 sidewalk;
		LaneAttributes_Barrier_t	 median;
		LaneAttributes_Striping_t	 striping;
		LaneAttributes_TrackedVehicle_t	 trackedVehicle;
		LaneAttributes_Parking_t	 parking;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LaneTypeAttributes_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LaneTypeAttributes;
extern asn_CHOICE_specifics_t asn_SPC_LaneTypeAttributes_specs_1;
extern asn_TYPE_member_t asn_MBR_LaneTypeAttributes_1[8];
extern asn_per_constraints_t asn_PER_type_LaneTypeAttributes_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _LaneTypeAttributes_H_ */
#include <asn_internal.h>
