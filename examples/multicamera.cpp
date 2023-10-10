#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
using namespace cv;
using namespace std;

typedef signed   char      int8_t;
typedef unsigned char      uint8_t;

typedef signed   short     int16_t;
typedef unsigned short     uint16_t;

typedef struct timeval TIME;
typedef struct{
    int device_handle;
    uint16_t chip_address;
    uint16_t register_address;
    uint16_t register_value;
}I2C_RW_STATE;

typedef struct{
 int slave_id;
 uint16_t sensor_id_addr; 
 uint16_t sensor_id_val;
 char* sensor_name;
}SENSOR_TYPE;
typedef struct{
    int gpio;
    int value;
}GPIO_TYPE;
typedef struct{
    bool camera_exist;
    char channel_id;
    char *camera_id;
    char *i2c_command; 
    GPIO_TYPE gpios[3];
}MULTI_ADAPTER_T;

I2C_RW_STATE i2c_state;

MULTI_ADAPTER_T arducam_adapter_board[]={
    {
        .channel_id = 0,
        .camera_id = "A",
        .i2c_command = "i2cset -y 1 0x70 0x00 0x04",
        .gpios = {
            {   .gpio =  7,
                .value = 0,
            },
            {   .gpio =  0,
                .value = 0,
            },
            {   .gpio =  1,
                .value = 1,
            }
        }
    },
    {
        .channel_id = 1,
        .camera_id = "B",
        .i2c_command = "i2cset -y 1 0x70 0x00 0x05",
        .gpios = {
            {   .gpio  = 7,
                .value = 1,
            },
            {   .gpio  = 0,
                .value = 0,
            },
            {   .gpio  = 1,
                .value = 1,
            }
        }
    },
    {
        .channel_id = 2,
        .camera_id = "C",
        .i2c_command = "i2cset -y 1 0x70 0x00 0x06",
        .gpios = {
            {   .gpio  = 7,
                .value = 0,
            },
            {   .gpio  = 0,
                .value = 1,
            },
            {   .gpio  = 1,
                .value = 0,
            }
        }
    },

};

const SENSOR_TYPE arducam_sensor_list[] = {
    {   .slave_id = 0x10, 
        .sensor_id_addr = 0x0000,
        .sensor_id_val = 0x0219,
        .sensor_name ="imx219"
    },
    {
        .slave_id = 0x36, 
        .sensor_id_addr = 0x300A,
        .sensor_id_val = 0x5647,
        .sensor_name ="ov5647" 
    },
    {
        .slave_id = 0x1A, 
        .sensor_id_addr = 0x0016,
        .sensor_id_val = 0x0477,
        .sensor_name ="imx477" 
    }
};

int i2c_read_reg(I2C_RW_STATE *state)
{ 
    uint16_t val = 0;
    int err;
	uint8_t buf[2];
    uint8_t data[1];
    if (ioctl(state->device_handle, I2C_SLAVE_FORCE, state->chip_address) < 0)
	{
		printf("Failed to set I2C address\n");
        return -1;
	}
    buf[0] = state->register_address >> 8;
    buf[1] = state->register_address;
	struct i2c_rdwr_ioctl_data msgset;
	struct i2c_msg msgs[2] = {
		{
			 .addr = state->chip_address,
			 .flags = 0,
			 .len = 2,
			 .buf = buf,
		},
		{
			.addr = state->chip_address,
			.flags = I2C_M_RD,
			.len = 1,
			.buf = data,
		},
	};
	msgset.msgs = msgs;
	msgset.nmsgs = 2;
    err = ioctl(state->device_handle, I2C_RDWR, &msgset);
	if (err != (int)msgset.nmsgs){
        return -1;  
    }		
    state->register_value = data[0];
	return 0;
}

int  camera_detect(){
   int i = 0;
   uint16_t sensorID = 0;
   for(i = 0; i< sizeof(arducam_sensor_list)/sizeof(arducam_sensor_list[0]); i++){
       sensorID = 0;
       i2c_state.chip_address =arducam_sensor_list[i]. slave_id;
       i2c_state.register_address =arducam_sensor_list[i].sensor_id_addr; //read hight id
       i2c_read_reg(&i2c_state); sensorID = i2c_state.register_value;
       i2c_state.register_address =arducam_sensor_list[i].sensor_id_addr+1; //read low id
       i2c_read_reg(&i2c_state); sensorID = sensorID<<8 | i2c_state.register_value;
       if(sensorID == arducam_sensor_list[i].sensor_id_val){
           return sensorID;
       }
   }
    return 0;
}

void initGPIO(){
    wiringPiSetup() ;
	for(int i = 0 ; i < sizeof(arducam_adapter_board[0].gpios)/sizeof(arducam_adapter_board[0].gpios[0]) ; i++){
		pinMode (arducam_adapter_board[0].gpios[i].gpio, OUTPUT);
	}
}
int init_camera(VideoCapture &cap){
    int access(const char *filename, int mode);
    char *i2c;
    int ret = 0;
    system("sudo modprobe bcm2835_v4l2");
    if(access("/dev/video0",0)){
        printf("Please check your camera connection,then try again.\r\n");
        exit(0);
    }
    sleep(0.1);
    for(char ch_id = 0; ch_id < sizeof(arducam_adapter_board)/sizeof(arducam_adapter_board[0]);ch_id++){  
        i2c = arducam_adapter_board[ch_id].i2c_command;
        system(i2c);
        for(int i = 0; i< sizeof(arducam_adapter_board[ch_id].gpios)/sizeof(arducam_adapter_board[ch_id].gpios[0]);i++ ){
            digitalWrite(arducam_adapter_board[ch_id].gpios[i].gpio,arducam_adapter_board[ch_id].gpios[i].value);  
        }
        if(camera_detect()){
           printf("Detected camera on channal:%s\r\n",arducam_adapter_board[ch_id].camera_id);
           cap.open(0);
            if (!cap.isOpened())
            {
                printf("ERROR! Unable to open camera\r\n") ;
                return -1;
            } 

            cap.set(CV_CAP_PROP_FRAME_WIDTH,320);
            cap.set(CV_CAP_PROP_FRAME_HEIGHT,240);
            cap.set(CV_CAP_PROP_FPS, 60); 
            cap.grab();
            system("v4l2-ctl -c exposure_time_absolute=1000");
            arducam_adapter_board[ch_id].camera_exist = 1;
            ret ++;
            sleep(1);

        } else{
            arducam_adapter_board[ch_id].camera_exist = 0;
           printf("Camera is missing on channal:%s\r\n",arducam_adapter_board[ch_id].camera_id); 
        } 
       
    }
    printf("Start preview...\r\n");
    return ret;
}

void choose_channal(int ch_id){
    for(int i = 0; i< sizeof(arducam_adapter_board[ch_id].gpios)/sizeof(arducam_adapter_board[ch_id].gpios[0]); i++ ){
            digitalWrite(arducam_adapter_board[ch_id].gpios[i].gpio,arducam_adapter_board[ch_id].gpios[i].value);  
        }
}


TIME currentTimeMillis(){
    struct timeval start;
    gettimeofday( &start, NULL );
    return start;
}
int main(int, char **)
{
    Mat surface;
    Mat frame;
    Mat frameA, frameB, frameC;

    std::string prefixA = "sourceA_";
    std::string prefixB = "sourceB_";
    std::string prefixC = "sourceC_";
    
    std::string subfix = ".png"; 

    VideoCapture cap; 
    
    int fd = open ("/dev/i2c-1",O_RDWR);
    if(!fd){
        printf("Couldn't open i2c device, please enable the i2c1 firstly\r\n");
        return -1;
    }
    i2c_state.device_handle = fd;
    initGPIO();
    if(init_camera(cap) == 0){
        printf("Can't find any canera\r\n");
        exit(0);
    }
    long begin_time = time(NULL);
    int totalFrame = 0;
    int flag = 0;
    int count = 0;
    int ret;



    for(int i=0;i<40;i++)
    {
        
        if(arducam_adapter_board[flag].camera_exist)
        {
            std::cout<<"start successfully"<<std::endl;
            choose_channal(flag);
     
        

            if(cap.isOpened()){

                std::cout<<flag<<std::endl;
                if(flag==0)
                   cap.grab();
                {  ret = cap.read(frameA);
                }

                if(flag==1)
                   cap.grab();
                {  ret = cap.read(frameB);
                }

                if(flag==2)
                   cap.grab();
                {  ret = cap.read(frameC);
                }
                //

            }else{
                std::cout<<"fails"<<std::endl;	
            }
                
        }

        if(flag==0 && ret==1)
        {    
            std::cout<<prefixA+std::to_string(i)+subfix<<std::endl;
            cv::imwrite(prefixA+std::to_string(i)+subfix, frameA);        
        }

        if(flag==1 && ret==1)
        {    
            std::cout<<prefixB+std::to_string(i)+subfix<<std::endl;
            cv::imwrite(prefixB+std::to_string(i)+subfix, frameB);        
        }

        if(flag==2 && ret==1)
        {    
            std::cout<<prefixC+std::to_string(i)+subfix<<std::endl;
            cv::imwrite(prefixC+std::to_string(i)+subfix, frameC);        
        }
        
        
 
         




      flag = (flag+1)%3; 

    

    }

    // cv::imwrite("source_1.png", frameB);

    return 0;
}
