#ifndef GPS_H
#define GPS_H
typedef struct{
    char string[25];
}gps_term_t;

typedef struct {
    int max_term;
    gps_term_t term[25];
}gps_frame_t;

typedef enum {
    GGA,
    GSV,
    RMC,
    GSA,
    UNKNOWN
}gps_header_t;

typedef struct {
    int fix_time;
    double lat;
    char lat_loc;
    double lon;
    char lon_loc;
    int fix_quality;
    int num_sats;
    double dilution;
    double altitude;
    double geoid;
    int time;
    int id;
    int checksum;
}gga_frame_t;

typedef struct {
    int fix_time;
    char status;
    double lat;
    char lat_loc;
    double lon;
    char lon_loc;
    double speed;
    double angle;
    int date;
    double magnetic;
    char magnetic_loc;
    int checksum;
}rmc_frame_t;

typedef struct {
    int num_sentence;
    int s_num;
    int num_sats;
    int prn[4];
    int elev[4];
    int azimuth[4];
    int snr[4];
    int checksum;
}gsv_frame_t;

typedef struct {
    char selection;
    int fix_type;
    int prn[12];
    double PDOP;
    double HDOP;
    double VDOP;
    int checksum;
}gsa_frame_t;

gps_header_t check_header(gps_frame_t *gps_frame);
int cal_checksum(char * s);
int get_checksum(char * s);
void process_gps_data(char *s, gps_frame_t * frame);
void parse_gps_sentence(gps_frame_t *gps_frame);

#endif
