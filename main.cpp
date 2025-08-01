#include <cstdlib>
#include <iostream>
#include <cstring>
#include "utils.cpp"

int main(int argc, char *argv[]) {
    if ((argc > 2) && (argc <= 4)) {
        int status = libusb_init(NULL);
        if (status != 0) {
            std::cerr << "Error cold't intit libusb" << std::endl;
            return -status;
        }
        //libusb_set_debug(NULL, LIBUSB_LOG_LEVEL_DEBUG);
        libusb_device_handle *dac = libusb_open_device_with_vid_pid(NULL, MDDP_VID, MDDP_PID);
        if (!dac) {
            std::cerr << "dac not connected" << std::endl;
            return -1;
        }
        if (strcmp(argv[1], "get") == 0){
            if (strcmp(argv[2], "all") == 0){
                std::array<uint8_t, DATA_BUFFER_SIZE> data = read(dac, GET_ALL);
                std::cout << "Volume: " << get_volume(dac) << std::endl;
                std::cout << "Filter: " << filterTable[data[FILTER_IDX]] << std::endl;
                if (data[GAIN_IDX] == 0) { std::cout << "Gain: " << "Low" << std::endl; }
                else { std::cout << "Gain: " << "High" << std::endl; };
                std::cout << "Indicator: " << get_indicator(dac) << std::endl;
            } else if (strcmp(argv[2], "volume") == 0) {
                std::cout << get_volume(dac) << std::endl;
            } else if (strcmp(argv[2], "filter") == 0) {
                std::cout << get_filter(dac) << std::endl;
            }else if (strcmp(argv[2], "gain") == 0) {
                std::cout << get_gain(dac) << std::endl;
            } else if (strcmp(argv[2], "indicator") == 0) {
                std::cout << get_indicator(dac) << std::endl;
            } else {
                std::cerr << "Invalid command usage: " << argv[0] << " get <all|volume|filter|gain|indicator>" << std::endl;
            }
        } else if (strcmp(argv[1], "set") == 0){
            if (strcmp(argv[2], "volume") == 0){
                set_volume(dac,argv[3]);
            } else if (strcmp(argv[2], "filter") == 0){
                set_filter(dac,argv[3]);
            } else if (strcmp(argv[2], "gain") == 0){
                set_gain(dac, argv[3]);
            } else if (strcmp(argv[2], "indicator") == 0){
                set_indicator(dac, argv[3]);
            } else {
                std::cout << "invalid command usage: " << argv[0] << " set <volume|filter|gain|indicator>" << std::endl;
            }
        } else {
            std::cerr << "Invalid command usage: " << argv[0] << " get|set|help" << std::endl;
            std::cout << "Commands:" << std::endl;
            std::cout << "  get [all|volume|filter|gain|indicator]" << std::endl;
            std::cout << "  set [volume|filter|gain|indicator] [value]" << std::endl;
            std::cout << "  help - Show this help message." << std::endl << std::endl;
            std::cout << "Parameters and their values:" << std::endl;
            std::cout << "  volume - Volume level (0-60)" << std::endl;
            std::cout << "  filter - Filter type (0-4):" << std::endl;
            std::cout << "    0: Fast Roll Off Low Latency" << std::endl;
            std::cout << "    1: Fast Roll Off Phase Compensated" << std::endl;
            std::cout << "    2: Slow Roll Off Low Latency" << std::endl;
            std::cout << "    3: Slow Roll Off Phase Compensated" << std::endl;
            std::cout << "    4: Non Oversampling" << std::endl;
            std::cout << "  gain - Gain level (0 or 1):" << std::endl;
            std::cout << "    0: Low" << std::endl;
            std::cout << "    1: High" << std::endl;
            std::cout << "  indicator - Indicator status (0, 1, or 2):" << std::endl;
            std::cout << "    0: on" << std::endl;
            std::cout << "    1: Temp off" << std::endl;
            std::cout << "    2: Off" << std::endl;
        }
        libusb_close(dac);
        libusb_exit(NULL);
        return 0;
    } else {
        std::cerr << "Invalid command usage: " << argv[0] << " get|set|help" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  get [all|volume|filter|gain|indicator]" << std::endl;
        std::cout << "  set [volume|filter|gain|indicator] [value]" << std::endl;
        std::cout << "  help - Show this help message." << std::endl << std::endl;
        std::cout << "Parameters and their values:" << std::endl;
        std::cout << "  volume - Volume level (0-60)" << std::endl;
        std::cout << "  filter - Filter type (0-4):" << std::endl;
        std::cout << "    0: Fast Roll Off Low Latency" << std::endl;
        std::cout << "    1: Fast Roll Off Phase Compensated" << std::endl;
        std::cout << "    2: Slow Roll Off Low Latency" << std::endl;
        std::cout << "    3: Slow Roll Off Phase Compensated" << std::endl;
        std::cout << "    4: Non Oversampling" << std::endl;
        std::cout << "  gain - Gain level (0 or 1):" << std::endl;
        std::cout << "    0: Low" << std::endl;
        std::cout << "    1: High" << std::endl;
        std::cout << "  indicator - Indicator status (0, 1, or 2):" << std::endl;
        std::cout << "    0: on" << std::endl;
        std::cout << "    1: Temp off" << std::endl;
        std::cout << "    2: Off" << std::endl;
    }
}
