#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <iterator>
#include <nan.h>
#include <node.h>
#include <node_buffer.h>

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;


    using namespace Nan;
    using namespace v8;//*/
    using namespace std;

    auto place_bit(int target, int position, int value) {
        return ((target & ~(1 << position)) | (value << position));
    }

    uint8_t *get_pixel8_block(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition);

    void pattern_convert(uint8_t *arr, int length);

    unsigned int char_arr_to_int(char arr[], int length);

    float get_a_factor(uint8_t *arr, int length);

    void
    write_pixel8_block(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition, char *data);

    int get_bit(int target, int position) {
        return ((target >> position) & 1);
    }

    void delete_2d_arr(char **arr, int rowamt) {
        for (int i = 0; i < rowamt; i++) {
            delete[] arr[i];
        };
        delete[] arr;
    }

    struct lineqn {
        float B;
        float M;

        lineqn(int x1, float y0, float y1) {
            M = (y1 - y0) / (float) x1;
            B = y0;
           // cout << "BUILT";
        }

        float get(int x) {
            return M * (float) x + float(B);
        }
    };


//VERY questionable format, should revise probably
    struct dataobject {
        size_t size;
        char *arr;

        dataobject() {
            //   arr = new char;
            //    size = 0;
        };

        dataobject(size_t sz) {
            arr = new char[sz];
            size = sz;
        }

        dataobject(char *a, size_t sz) {
            arr = a;
            size = sz;
        }

        char operator[](unsigned int b) {
            return arr[b];
        }
    };

    unsigned int WIDTH, HEIGHT, PIXEL_ARR_POINTER, BIT_PER_PIXEL, BYTE_PER_ROW;
    double BYTE_PER_PIXEL;
    fstream FILEE;


    void identify_complexity(int block_height) {
        int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
        int block_width_amount = (int) floor(
                (double) WIDTH / 8.00); // width amount of pixels(1 bit per pixel)/8 bits per read block
        //cout << block_height_amount << " " << block_width_amount;
        for (int i = 0; i < block_height_amount; i++) {
            for (int j = 0; j < block_width_amount; j++) {
                uint8_t *status = get_pixel8_block(block_height, j * 8, i * block_height, 0, 0);
                //   cout << get_a_factor(status, block_height) << "   ";
                delete[] status;
            }
            //cout<<"\n";
        }
    }

    //returns length of container
    dataobject read_BPCS(float a_threshold_block, float a_threshold_header, int block_height) {
        unsigned int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
        unsigned int block_width_amount = (int) floor(
                (double) WIDTH / 8.00); // width amount of pixels(1 bit per pixel)/8 bits per read block
        unsigned int HeadBlockerSize = ceil(4.00 / (float) block_height);
        char *HB = new char[4];
        // uint8_t * container = new uint8_t[2];
        unsigned int iter_length; //match with byte iter
        unsigned int iter_length2;
        unsigned int iter_length3; //match with block iter
        unsigned int BLOCK_ITER = 0;
        unsigned int BYTE_ITER = 0;
        dataobject output;
        bool *FLIPYN;
        unsigned int HB_COUNT = 0;
        float efe = a_threshold_header;
        bool read_headerbytes = false;
        for (int L = 0; L < 8; L++) { //bit significance iteration, up to a noise limit
            //  L = 7;
            //cout << BIT_PER_PIXEL;
            for (int k = 0; k < (BIT_PER_PIXEL / 8); k++) { //iteration of selected color spectrum
                //     cout << "HEE\n";

                for (int i = 0; i < block_height_amount; i++) {
                    //  cout << "HE\n";

                    for (int j = 0; j < block_width_amount; j++) {
                        //      cout << FILEE.fail();
                        //    cout << BYTE_ITER << "\n";
                        //  cout << (read_headerbytes == false ? a_threshold_header : a_threshold_block) << " ";
                        //  uint8_t * tmpdel = container;
                        //  int eeee = 0;
                        uint8_t *container = get_pixel8_block(block_height, j * 8, i * block_height, k, L);
                        // delete[] tmpdel;
                        float v = get_a_factor(container, block_height);
                        //    cout << "\n" << v;
                        //    cout << "\nHB: " << read_headerbytes << " " << HB_COUNT<< "\n";
                        if (read_headerbytes && HB_COUNT < 5) {
                            //   cout << "BLOCK: " << j*8 << "  " << i*block_height << "\n";

                            for (int m = 0; m < block_height; m++) {
                                //     cout << "\n" << bitset<8>(container[m]) << " " << m << "\n";

                                if (HB_COUNT < 4) {
                                    HB[HB_COUNT] = container[m];
                                    //   cout << bitset<8>(container[m]) << " " << j * 8 << " " << i * block_height << "\n";
                                    HB_COUNT++;
                                    if (HB_COUNT == 4) {
                                        //HB_COUNT++;
                                        iter_length = char_arr_to_int(HB, 4);
                                        iter_length2 = ceil((float) iter_length / (float) block_height);
                                        iter_length3 = iter_length2;
                                        //      cout << iter_length << " I2: " << iter_length2;
                                        FLIPYN = new bool[iter_length3];
                                        output.arr = new char[iter_length];
                                        output.size = iter_length;
                                        efe = a_threshold_block;
                                    }
                                } else {
                                    //     cout << "\nHERE";
                                    for (int bs = 0; bs < 8; bs++) {
                                        FLIPYN[iter_length3 - iter_length2] = get_bit(container[m], bs);
                                        //   cout << FLIPYN[iter_length3 - iter_length2];
                                        iter_length2--;
                                        if (iter_length2 == 0) {
                                            HB_COUNT++;
                                            //for (int ie = 0; ie < iter_length3; ie++){
                                            //    cout << FLIPYN[ie];
                                            //}
                                            break;
                                        }
                                    }
                                }
                            }
                        } else if (v > efe) { //success: sufficiently noisy to be data block
                            // cout << "HO";
                            if (HB_COUNT < 4 && !read_headerbytes) { read_headerbytes = true; }
                            else {
                                if (FLIPYN[BLOCK_ITER]) pattern_convert(container, block_height);
                                // cout<<" "<< iter_length2 << "e " << BLOCK_ITER;
                                for (int bl = 0; bl < block_height; bl++) {
                                    //        cout << bl;
                                    //       cout << "\nE" << BYTE_ITER << " " << bitset<8>(container[bl]);
                                    // cout << (char)container[bl];
                                    output.arr[BYTE_ITER] = container[bl];
                                    BYTE_ITER++;
                                    if (BYTE_ITER == iter_length) {//cout << "RT";
                                        //  cout << "DONE";
                                        return output;
                                    }
                                    //        cout << "RT";
                                }
                                BLOCK_ITER++;
                            }
                        }
                        //   cout << "TEST";
                    }
                }
            }
        }
        return output;
    }

    void write_BPCS(float a_threshold, int block_height, char *content, int content_size, int digit_limit,
                    float wr_data_threshold) {
        int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
        int block_width_amount = (int) floor(
                (double) WIDTH / 8.f); // width amount of pixels(1 bit per pixel)/8 bits per read block
        //  cout << block_height_amount << " " << block_width_amount;
        unsigned int HeadBlockerSize = ceil((4.f + ceil((double) content_size / 8.f)) / (double) block_height);
        //cout << HeadBlockerSize;
        unsigned int HBcheck = HeadBlockerSize;
        unsigned int **Header_PTR_ARR = new unsigned int *[HeadBlockerSize];
        unsigned int content_ind = 0;
        uint8_t *evaluator = new uint8_t[2];
        //cout << block_height * HeadBlockerSize << " " << HeadBlockerSize << " " << block_height<< " ";
        unsigned int arrsz = HeadBlockerSize * block_height; //amount of blocks * amount of bytes per block
        char *RV = new char[arrsz];
        for (int a = 0; a < 4; a++) {
            RV[a] = ((content_size >> (a * 8)) & 0xFF); //mask last 8 bits, rightshift by a*8 bits
        }
        //int ctr = 0;
        int ctr_A_byte = 0;
        int ctr_A_bit = 0;
        bool HB_BLOCK_ENB = false;

        for (int L = 0; L < digit_limit; L++) { //bit significance iteration, up to a significance limit
            //    L = 7;
            // cout<< "T";
            for (int k = 0; k < BIT_PER_PIXEL / 8; k++) { //iteration of selected color spectrum
                //cout<< "e";

                for (int i = 0; i < block_height_amount; i++) {
                    // cout<< "Tf";

                    for (int j = 0; j < block_width_amount; j++) {
                        uint8_t *tmpdel = evaluator;
                        evaluator = get_pixel8_block(block_height, j * 8, i * block_height, k, L);
                        delete[] tmpdel;
                        float v = get_a_factor(evaluator, block_height);
                        //cout << "\nWBPCS: " << v;
                        if (HB_BLOCK_ENB && HBcheck > 0) {
                            int addr = HeadBlockerSize - HBcheck;
                            Header_PTR_ARR[addr] = new unsigned int[4];
                            Header_PTR_ARR[addr][0] = j * 8;
                            Header_PTR_ARR[addr][1] = i * block_height;
                            Header_PTR_ARR[addr][2] = k;
                            Header_PTR_ARR[addr][3] = L;
                            //  for(int ij = 0; ij < 4; ij++){
                            //     cout << " " << Header_PTR_ARR[addr][ij];
                            // }
                            HBcheck--;
                            continue; //continue to next block, no checking
                        }
                        if (v > a_threshold) { //success: sufficiently noisy to use
                            if (HBcheck > 0 && !HB_BLOCK_ENB) { //reserve header blocks
                                HB_BLOCK_ENB = true;
                                continue; //continue to next block
                            } else {
                                //    cout << "\n" <<(content_size - content_ind) << "\n";
                                if (ctr_A_bit == 0) RV[4 + ctr_A_byte] = (uint8_t) (rand() % 256);
                                if ((content_size - content_ind) <
                                    block_height) { //insufficient data left for block: generate random noise in remaining space
                                    //     cout << "\nRGR";
                                    char *randgenarr = new char[block_height];
                                    memcpy(randgenarr, &content[content_ind], content_size - content_ind);
                                    for (int m = (content_size - content_ind); m < block_height; m++) {
                                        randgenarr[m] = (char) (rand() %
                                                                256); //replacement of empty blocks with random data
                                    }
                                    for (int ie = 0; ie < block_height; ie++) {
                                        //     cout << randgenarr[ie];
                                    }
                                    int flipbit = 0;
                                    if (get_a_factor((uint8_t *) randgenarr, block_height) < wr_data_threshold) {
                                        pattern_convert((uint8_t *) randgenarr,
                                                        block_height); //written data failed a check - invert to exceed threshold
                                        flipbit = 1;
                                        //           cout << "FLP";
                                    }
                                    RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                    //     cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                    ctr_A_bit++;
                                    if (ctr_A_bit == 8) {
                                        ctr_A_bit = 0;
                                        ctr_A_byte++;
                                    }
                                    write_pixel8_block(block_height, j * 8, i * block_height, k, L,
                                                       randgenarr); //write to block
                                    delete[] randgenarr;
                                    goto done;
                                } else {
                                    //     cout << "\nNORM";
                                    int flipbit = 0;
                                    for (int ie = 0; ie < block_height; ie++) {
                                        //       cout << content[content_ind+ie];
                                    }
                                    //  cout << get_a_factor((uint8_t*)&content[content_ind], block_height);
                                    if (get_a_factor((uint8_t *) &content[content_ind], block_height) <
                                        wr_data_threshold) {
                                        // cout << "FLP";
                                        pattern_convert((uint8_t *) &content[content_ind],
                                                        block_height); //written data failed a check - invert to exceed threshold
                                        flipbit = 1;
                                    }
                                    RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                    //cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                    ctr_A_bit++;
                                    if (ctr_A_bit == 8) {
                                        ctr_A_bit = 0;
                                        ctr_A_byte++;
                                    }
                                    write_pixel8_block(block_height, j * 8, i * block_height, k, L,
                                                       &content[content_ind]); //write to block
                                    content_ind += block_height;
                                    if ((content_size - content_ind) == 0) goto done;
                                }
                            }
                        }
                    }
                }
            }
        }
        done:
        //cout << "\nOUT";
        //cout << arrsz/8;
        int ctr = 0;
        for (int d = 4 + (int) ceil((float) content_size / 8.f); d < arrsz; d++) {
            // RV[d] = rand() % 256; //fill remaining block space with noise
        }
        // cout << FILEE.fail();
        for (int i = 0; i < arrsz; i++) {
            //    cout << std::bitset<8>(RV[i]) << "\n";
        }
        // cout << "\n";
        // cout << "\n";

        for (int c = 0; c < arrsz / block_height; c++) {
            write_pixel8_block(block_height, Header_PTR_ARR[c][0], Header_PTR_ARR[c][1], Header_PTR_ARR[c][2],
                               Header_PTR_ARR[c][3], &RV[c * block_height]);
            //     cout << Header_PTR_ARR[c][0] << " " << Header_PTR_ARR[c][1] << "\n";
            //    cout << "\n";
            for (int i = c * block_height; i < arrsz; i++) {
                //    cout << std::bitset<8>(RV[i]) << "\n";
            }
        }

        if ((content_size - content_ind) != 0)
            cout << "Insufficient space. " << content_ind << " bytes out of " << content_size << " were written.";

    }

    void write_BPCS(lineqn a_threshold, int block_height, char *content, int content_size, int digit_limit,
                    float wr_data_threshold) {
        int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
        int block_width_amount = (int) floor(
                (double) WIDTH / 8.f); // width amount of pixels(1 bit per pixel)/8 bits per read block
        //  cout << block_height_amount << " " << block_width_amount;
        unsigned int HeadBlockerSize = ceil((4.f + ceil((double) content_size / 8.f)) / (double) block_height);
        //cout << HeadBlockerSize;
        unsigned int HBcheck = HeadBlockerSize;
        unsigned int **Header_PTR_ARR = new unsigned int *[HeadBlockerSize];
        unsigned int content_ind = 0;
        uint8_t *evaluator = new uint8_t[2];
        //cout << block_height * HeadBlockerSize << " " << HeadBlockerSize << " " << block_height<< " ";
        unsigned int arrsz = HeadBlockerSize * block_height; //amount of blocks * amount of bytes per block
        char *RV = new char[arrsz];
        for (int a = 0; a < 4; a++) {
            RV[a] = ((content_size >> (a * 8)) & 0xFF); //mask last 8 bits, rightshift by a*8 bits
        }
        //int ctr = 0;
        int ctr_A_byte = 0;
        int ctr_A_bit = 0;
        bool HB_BLOCK_ENB = false;

        for (int L = 0; L < digit_limit; L++) { //bit significance iteration, up to a significance limit
            //    L = 7;
            // cout<< "T";
            for (int k = 0; k < BIT_PER_PIXEL / 8; k++) { //iteration of selected color spectrum
                //cout<< "e";

                for (int i = 0; i < block_height_amount; i++) {
                    // cout<< "Tf";

                    for (int j = 0; j < block_width_amount; j++) {
                        uint8_t *tmpdel = evaluator;
                        evaluator = get_pixel8_block(block_height, j * 8, i * block_height, k, L);
                        delete[] tmpdel;
                        float v = get_a_factor(evaluator, block_height);
                        //cout << "\nWBPCS: " << v;
                        if (HB_BLOCK_ENB && HBcheck > 0) {
                            int addr = HeadBlockerSize - HBcheck;
                            Header_PTR_ARR[addr] = new unsigned int[4];
                            Header_PTR_ARR[addr][0] = j * 8;
                            Header_PTR_ARR[addr][1] = i * block_height;
                            Header_PTR_ARR[addr][2] = k;
                            Header_PTR_ARR[addr][3] = L;
                            //  for(int ij = 0; ij < 4; ij++){
                            //     cout << " " << Header_PTR_ARR[addr][ij];
                            // }
                            HBcheck--;
                            continue; //continue to next block, no checking
                        }
                        if (v > a_threshold.get(L)) { //success: sufficiently noisy to use
                            if (HBcheck > 0 && !HB_BLOCK_ENB) { //reserve header blocks
                                HB_BLOCK_ENB = true;
                                continue; //continue to next block
                            } else {
                                //    cout << "\n" <<(content_size - content_ind) << "\n";
                                if (ctr_A_bit == 0) RV[4 + ctr_A_byte] = (uint8_t) (rand() % 256);
                                if ((content_size - content_ind) <
                                    block_height) { //insufficient data left for block: generate random noise in remaining space
                                    //     cout << "\nRGR";
                                    char *randgenarr = new char[block_height];
                                    memcpy(randgenarr, &content[content_ind], content_size - content_ind);
                                    for (int m = (content_size - content_ind); m < block_height; m++) {
                                        randgenarr[m] = (char) (rand() %
                                                                256); //replacement of empty blocks with random data
                                    }
                                    for (int ie = 0; ie < block_height; ie++) {
                                        //     cout << randgenarr[ie];
                                    }
                                    int flipbit = 0;
                                    if (get_a_factor((uint8_t *) randgenarr, block_height) < wr_data_threshold) {
                                        pattern_convert((uint8_t *) randgenarr,
                                                        block_height); //written data failed a check - invert to exceed threshold
                                        flipbit = 1;
                                        //           cout << "FLP";
                                    }
                                    RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                    //     cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                    ctr_A_bit++;
                                    if (ctr_A_bit == 8) {
                                        ctr_A_bit = 0;
                                        ctr_A_byte++;
                                    }
                                    write_pixel8_block(block_height, j * 8, i * block_height, k, L,
                                                       randgenarr); //write to block
                                    delete[] randgenarr;
                                    goto done;
                                } else {
                                    //     cout << "\nNORM";
                                    int flipbit = 0;
                                    for (int ie = 0; ie < block_height; ie++) {
                                        //       cout << content[content_ind+ie];
                                    }
                                    //  cout << get_a_factor((uint8_t*)&content[content_ind], block_height);
                                    if (get_a_factor((uint8_t *) &content[content_ind], block_height) <
                                        wr_data_threshold) {
                                        // cout << "FLP";
                                        pattern_convert((uint8_t *) &content[content_ind],
                                                        block_height); //written data failed a check - invert to exceed threshold
                                        flipbit = 1;
                                    }
                                    RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                    //cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                    ctr_A_bit++;
                                    if (ctr_A_bit == 8) {
                                        ctr_A_bit = 0;
                                        ctr_A_byte++;
                                    }
                                    write_pixel8_block(block_height, j * 8, i * block_height, k, L,
                                                       &content[content_ind]); //write to block
                                    content_ind += block_height;
                                    if ((content_size - content_ind) == 0) goto done;
                                }
                            }
                        }
                    }
                }
            }
        }
        done:
        //cout << "\nOUT";
        //cout << arrsz/8;
        int ctr = 0;
        for (int d = 4 + (int) ceil((float) content_size / 8.f); d < arrsz; d++) {
            // RV[d] = rand() % 256; //fill remaining block space with noise
        }
        // cout << FILEE.fail();
        for (int i = 0; i < arrsz; i++) {
            //    cout << std::bitset<8>(RV[i]) << "\n";
        }
        // cout << "\n";
        // cout << "\n";

        for (int c = 0; c < arrsz / block_height; c++) {
            write_pixel8_block(block_height, Header_PTR_ARR[c][0], Header_PTR_ARR[c][1], Header_PTR_ARR[c][2],
                               Header_PTR_ARR[c][3], &RV[c * block_height]);
            //     cout << Header_PTR_ARR[c][0] << " " << Header_PTR_ARR[c][1] << "\n";
            //    cout << "\n";
            for (int i = c * block_height; i < arrsz; i++) {
                //    cout << std::bitset<8>(RV[i]) << "\n";
            }
        }

        if ((content_size - content_ind) != 0)
            cout << "Insufficient space. " << content_ind << " bytes out of " << content_size << " were written.";

    }


    unsigned int char_arr_to_int(char arr[], int length) {
        unsigned int output = 0;
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 8; j++) {
                output = place_bit(output, (i * 8) + j, get_bit(arr[i], j));
            }
        }
        //  cout << "\n" << bitset<32>(output);
        return output;
    }


    char *get_char_arr(char *ref, int location, int length) {
        // cout << location << FILEE.good() <<FILEE.fail();
        FILEE.seekg(location, ios::beg);
        FILEE.read(ref, length);
        return ref;
    }

    unsigned int get_int_value(int position, int length) {
        char *tmpV = new char[length];
        get_char_arr(tmpV, position, length);
        //  cout << bitset<8>(tmpV[0]) << "\n";
        unsigned int output = char_arr_to_int(tmpV, length);
        // cout << tmpV;
        delete[] tmpV;
        return output;
    }

    uint8_t get_byte(int position) {
        char tmpV = 0;
        //cout << FILEE.end;
        // cout << " " << position;
        FILEE.seekg(position, ios::beg);
        FILEE.read(&tmpV, 1);
        //    cout << position << " " <<FILEE.good() << FILEE.eof() << FILEE.fail() << FILEE.bad() << "\n";
        //cout << bitset<8>(tmpV);
        // int output = char_arr_to_int(tmpV, length);
        // cout << tmpV;
        return (uint8_t) tmpV;
    }

    void write_byte(char data, int position) {
        //cout << FILEE.end;
        // cout << " " << position;
        FILEE.seekp(position, ios::beg);
        FILEE.write(&data, 1);
        //    cout << position << " " <<FILEE.good() << FILEE.eof() << FILEE.fail() << FILEE.bad() << "\n";
        //cout << bitset<8>(tmpV);
        // int output = char_arr_to_int(tmpV, length);
        // cout << tmpV;
    }

    uint8_t get_bit_value_from_file(int position, int bit) {
        //    cout << "\nBYTE: " << bitset<8>(get_int_value(position, 1));
        return get_bit(get_byte(position), bit);
    }

    //NOTE: this doesn't care if out of file bounds, restrict this elsewhere
    //reads a 8*N segement of data
    uint8_t *get_pixel8_block(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition) {
        uint8_t *output = new uint8_t[pixel_height];
        int start_pos = PIXEL_ARR_POINTER + (int) (BYTE_PER_PIXEL * (double) X_offset) + (BYTE_PER_ROW * Y_offset);
        //int byte
        for (int y = 0; y < pixel_height; y++) {
            output[y] = 0;
            for (int bs = 0; bs < 8; bs++) {
                uint8_t BIT = get_bit_value_from_file(
                        start_pos + (y * BYTE_PER_ROW) + (int) ((double) bs * BYTE_PER_PIXEL) + byteposition,
                        bitposition);
                //cout << "BIT: " <<  BIT << " ";
                output[y] = place_bit(output[y], bs, BIT);
            }
            //  cout << "\n";
            // if(FILEE.fail()) cout <<"\nLINE: " << start_pos + (y * BYTE_PER_ROW) << " " << bitset<8>(output[y]);
        }
        return output;
    }

    //NOTE: pixel height = size of data for all data to be transferred
    void
    write_pixel8_block(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition, char *data) {
        int start_pos = PIXEL_ARR_POINTER + (int) (BYTE_PER_PIXEL * (double) X_offset) + (BYTE_PER_ROW * Y_offset);
        int bit_iter = 0;
        int byte_iter = 0;
        for (int y = 0; y < pixel_height; y++) {
            //[y] = 0;
            for (int bs = 0; bs < 8; bs++) {
                int pos = start_pos + (y * BYTE_PER_ROW) + (int) ((double) bs * BYTE_PER_PIXEL) + byteposition;
                char bit = get_byte(pos);
                bit = place_bit(bit, bitposition, get_bit(data[byte_iter], bit_iter));
                bit_iter++;
                if (bit_iter == 8) {
                    byte_iter++;
                    bit_iter = 0;
                }
                write_byte(bit, pos);
                //cout << "BIT: " <<  BIT << " ";
            }
            //  cout << "\n";
            // if(FILEE.fail()) cout <<"\nLINE: " << start_pos + (y * BYTE_PER_ROW) << " " << bitset<8>(output[y]);
        }
        //  return output;
    }

    //does an XOR on all pixels aganist a checkerboard pattern. This inverses your complexity(a) factor.
    void pattern_convert(uint8_t *arr, int length) {
        //cout << "\nPC";
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 8; j++) {
                int VAL = get_bit(arr[i], j) ^((i % 2) ? (j % 2) : ((j + 1) %
                                                                    2)); //series of if statements generates checkerboard. edges cause disruption which changes the A factor.
                //  cout << (((i % 2) != 0) ? (j % 2) : ((j+1) % 2)); //confirm if checkboard works. If even row generate on even columns, else generate 1 on odd columns
                arr[i] = place_bit(arr[i], j, VAL);
            }
            //   cout << "\n";
        }
        //  cout << "\n";
    }

    //computes the complexity factor, a of the object. Theoretically we could put this into pattern_convert but that sounds messy
    float get_a_factor(uint8_t *arr, int length) {
        //cout << "\nAF: ";
        //cout << arr[0];
        float MAX_LEN = ((length - 1) * 8) + (length *
                                              7); //the ideal case, the checkerboard, has (width-1)*height amount of row bit swaps, and (height-1)*width amount of column bit swaps
        float CUR_FLIP_AMT = 0; //a flip is defined as having the opposite bit along a row or column.
        for (int i = 0; i < length - 1; i++) {
            for (int k = 0; k < 8; k++) {
                if (get_bit(arr[i], k) != get_bit(arr[i + 1], k)) CUR_FLIP_AMT++;
            }
        }
        //this should be in one for loop with j but im lazy
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < 7; j++) {
                //   cout <<  get_bit(arr[i], j) << " " << get_bit(arr[i], j+1) << "  |   ";
                if (get_bit(arr[i], j) != get_bit(arr[i], j + 1)) CUR_FLIP_AMT++;
            }
            //   cout << "\n";
        }
        //cout << "\n" << CUR_FLIP_AMT << "  " << MAX_LEN << "\n";
        return CUR_FLIP_AMT / MAX_LEN;
    }


uint8_t * get_pixel8_block_ARR(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition, char* data) {
    uint8_t * output = new uint8_t[pixel_height];
 //   cout << "Hello";
   // cout << PIXEL_ARR_POINTER;
    unsigned int start_pos = PIXEL_ARR_POINTER + (int) (BYTE_PER_PIXEL * (double) X_offset) + (BYTE_PER_ROW * Y_offset);
    //int byte
   // cout << start_pos;
    for (int y = 0; y < pixel_height; y++) {
        output[y] = 0;
        for (int bs = 0; bs < 8; bs++) {
            uint8_t BIT = get_bit(
                    data[start_pos + (y * BYTE_PER_ROW) + (int) ((double) bs * BYTE_PER_PIXEL) + byteposition],
                    bitposition);
        //    cout << "BIT: " <<  BIT << " ";
            output[y] = place_bit(output[y], bs, BIT);
        }
      //    cout << "\n";
        // if(FILEE.fail()) cout <<"\nLINE: " << start_pos + (y * BYTE_PER_ROW) << " " << bitset<8>(output[y]);
    }
      //  cout << "END";
    return output;
}

void write_pixel8_block_ARR(int pixel_height, int X_offset, int Y_offset, int byteposition, int bitposition, char *data, char* tgtdata, int PIXEL_ARR_POINTER, int BYTE_PER_PIXEL, int BYTE_PER_ROW) {
    int start_pos = PIXEL_ARR_POINTER + (int) (BYTE_PER_PIXEL * (double) X_offset) + (BYTE_PER_ROW * Y_offset);
    int bit_iter = 0;
    int byte_iter = 0;
    for (int y = 0; y < pixel_height; y++) {
        //[y] = 0;
        for (int bs = 0; bs < 8; bs++) {
            int pos = start_pos + (y * BYTE_PER_ROW) + (int) ((double) bs * BYTE_PER_PIXEL) + byteposition;
            char bit = tgtdata[pos];
            bit = place_bit(bit, bitposition, get_bit(data[byte_iter], bit_iter));
            int v=get_bit(data[byte_iter], bit_iter);
            bit_iter++;
            if (bit_iter == 8) {
                byte_iter++;
                bit_iter = 0;
            }
            tgtdata[pos] = bit;
           // cout << std::
            //write_byte(bit, pos);
        }
        //  cout << "\n";
        // if(FILEE.fail()) cout <<"\nLINE: " << start_pos + (y * BYTE_PER_ROW) << " " << bitset<8>(output[y]);
    }
    //  return output;
}

//int BPR = 0;
bool WRITE(char* data, char* content, int content_size, int block_height, float initAfac, float finalAfac, float wr_data_threshold) {
    lineqn a_threshold(8, initAfac, finalAfac);
    PIXEL_ARR_POINTER = char_arr_to_int(&data[10], 4);
    WIDTH = char_arr_to_int(&data[18], 4);
    HEIGHT = char_arr_to_int(&data[22], 4);
    BIT_PER_PIXEL = char_arr_to_int(&data[28], 2);
    BYTE_PER_ROW = ceil(((double)WIDTH * ((double)BIT_PER_PIXEL / 8.00))/4.00) * 4;
    BYTE_PER_PIXEL = ((double)BIT_PER_PIXEL/8.0);//
    //cout<< PIXEL_ARR_POINTER;
    //BPR = BYTE_PER_ROW;
    int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
    int block_width_amount = (int) floor(
            (double) WIDTH / 8.f); // width amount of pixels(1 bit per pixel)/8 bits per read block
    //  cout << block_height_amount << " " << block_width_amount;
    unsigned int HeadBlockerSize = ceil((4.f + ceil((double) content_size / 8.f)) / (double) block_height);
    //cout << HeadBlockerSize;
    unsigned int HBcheck = HeadBlockerSize;
    unsigned int **Header_PTR_ARR = new unsigned int *[HeadBlockerSize];
    unsigned int content_ind = 0;
   // uint8_t *evaluator = new uint8_t[2];
    //cout << block_height * HeadBlockerSize << " " << HeadBlockerSize << " " << block_height<< " ";
    unsigned int arrsz = HeadBlockerSize * block_height; //amount of blocks * amount of bytes per block
    char *RV = new char[arrsz];
    for (int a = 0; a < 4; a++) {
        RV[a] = ((content_size >> (a * 8)) & 0xFF); //mask last 8 bits, rightshift by a*8 bits
    }
    //int ctr = 0;
    int ctr_A_byte = 0;
    int ctr_A_bit = 0;
    bool HB_BLOCK_ENB = false;

    for (int L = 0; L < 8; L++) { //bit significance iteration, up to a significance limit
          //  L = 7;
        // cout<< "T";
        for (int k = 0; k < BIT_PER_PIXEL / 8; k++) { //iteration of selected color spectrum
          //  cout<< "e";

            for (int i = 0; i < block_height_amount; i++) {
                // cout<< "Tf";

                for (int j = 0; j < block_width_amount; j++) {
                    //uint8_t *tmpdel = evaluator;
                    uint8_t * evaluator = get_pixel8_block_ARR(block_height, j * 8, i * block_height, k, L, data);
                    //delete[] tmpdel;
                    float v = get_a_factor(evaluator, block_height);
                    //cout << "\nWBPCS: " << v;
                    if (HB_BLOCK_ENB && (HBcheck > 0)) {
                        int addr = HeadBlockerSize - HBcheck;
                        Header_PTR_ARR[addr] = new unsigned int[4];
                        Header_PTR_ARR[addr][0] = j * 8;
                        Header_PTR_ARR[addr][1] = i * block_height;
                        Header_PTR_ARR[addr][2] = k;
                        Header_PTR_ARR[addr][3] = L;
                        for(int ij = 0; ij < 4; ij++){
                       //      cout << "HH " << Header_PTR_ARR[addr][ij] << "\n";
                        }
                        HBcheck--;
                        continue; //continue to next block, no checking
                    }
                  //  cout << a_threshold.get(L) << "\n";
                    if (v > a_threshold.get(L)) { //success: sufficiently noisy to use
                        if ((HBcheck > 0) && !HB_BLOCK_ENB) { //reserve header blocks
                            HB_BLOCK_ENB = true;
                         //   cout<<"HI";
                            continue; //continue to next block
                        } else {
                            //    cout << "\n" <<(content_size - content_ind) << "\n";
                            if (ctr_A_bit == 0) RV[4 + ctr_A_byte] = (uint8_t) (rand() % 256);
                            if ((content_size - content_ind) <
                                block_height) { //insufficient data left for block: generate random noise in remaining space
                                //     cout << "\nRGR";
                                char *randgenarr = new char[block_height];
                                memcpy(randgenarr, &content[content_ind], content_size - content_ind);
                                for (int m = (content_size - content_ind); m < block_height; m++) {
                                    randgenarr[m] = (char) (rand() %
                                                            256); //replacement of empty blocks with random data
                                }
                            //    for (int ie = 0; ie < block_height; ie++) {
                                    //     cout << randgenarr[ie];
                              //  }
                                int flipbit = 0;
                                if (get_a_factor((uint8_t *) randgenarr, block_height) < wr_data_threshold) {
                                    pattern_convert((uint8_t *) randgenarr,
                                                    block_height); //written data failed a check - invert to exceed threshold
                                    flipbit = 1;
                                    //           cout << "FLP";
                                }
                                RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                //     cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                ctr_A_bit++;
                                if (ctr_A_bit == 8) {
                                    ctr_A_bit = 0;
                                    ctr_A_byte++;
                                }
                                write_pixel8_block_ARR(block_height, j * 8, i * block_height, k, L,
                                                       randgenarr, data,PIXEL_ARR_POINTER, BYTE_PER_PIXEL, BYTE_PER_ROW); //write to block
                                delete[] randgenarr;
                                goto done;
                            } else {
                                //     cout << "\nNORM";
                                int flipbit = 0;
                               // for (int ie = 0; ie < block_height; ie++) {
                                    //       cout << content[content_ind+ie];
                               // }
                                //  cout << get_a_factor((uint8_t*)&content[content_ind], block_height);
                                if (get_a_factor((uint8_t *) &content[content_ind], block_height) <
                                    wr_data_threshold) {
                                    // cout << "FLP";
                                    pattern_convert((uint8_t *) &content[content_ind],
                                                    block_height); //written data failed a check - invert to exceed threshold
                                    flipbit = 1;
                                }
                                RV[4 + ctr_A_byte] = place_bit(RV[4 + ctr_A_byte], ctr_A_bit, flipbit);
                                //cout << "\n FB: " << flipbit << "  " << bitset<8> (RV[4 + ctr_A_byte]) << "\n";
                                ctr_A_bit++;
                                if (ctr_A_bit == 8) {
                                    ctr_A_bit = 0;
                                    ctr_A_byte++;
                                }
                                write_pixel8_block_ARR(block_height, j * 8, i * block_height, k, L,
                                                       &content[content_ind], data,PIXEL_ARR_POINTER, BYTE_PER_PIXEL, BYTE_PER_ROW); //write to block
                                content_ind += block_height;
                                if ((content_size - content_ind) == 0) goto done;
                            }
                        }
                    }
                    delete[] evaluator;
                }
            }
        }
    }
    done:
   // delete[] evaluator;
    //cout << "\nOUT";
    //cout << arrsz/8;
    int ctr = 0;
    for (int d = 4 + (int) ceil((float) content_size / 8.f); d < arrsz; d++) {
         RV[d] = rand() % 256; //fill remaining block space with noise
    }
    // cout << FILEE.fail();
   // for (int i = 0; i < arrsz; i++) {
          //  cout << std::bitset<8>(RV[i]) << "\n";
    //}
    // cout << "\n";
    // cout << "\n";

    for (int c = 0; c < arrsz / block_height; c++) {
       // cout << Header_PTR_ARR[c][0] << " " << Header_PTR_ARR[c][1] <<  " " << Header_PTR_ARR[c][2] <<" " << Header_PTR_ARR[c][3] << "\n";
        write_pixel8_block_ARR(block_height, Header_PTR_ARR[c][0], Header_PTR_ARR[c][1], Header_PTR_ARR[c][2],Header_PTR_ARR[c][3], &RV[c * block_height], data, PIXEL_ARR_POINTER, BYTE_PER_PIXEL, BYTE_PER_ROW);

        //    cout << "\n";
       // /for (int i = c * block_height; i < arrsz; i++) {
                //cout << std::bitset<8>(RV[i]) << "\n";
      // / }
    }
    delete[] RV;
    delete_2d_arr((char**)Header_PTR_ARR, HeadBlockerSize);
    //cout << PIXEL_ARR_POINTER << " " << BYTE_PER_PIXEL;

  //  uint8_t * arr = get_pixel8_block_ARR(block_height, 8, 0, 0, 0, data, PIXEL_ARR_POINTER, BYTE_PER_PIXEL, BYTE_PER_ROW);

   // for(int i = 0; i < 8; i++){
   //     cout<<bitset<8>(arr[i]) << "\n";
   // }
   // for(int i =0 ; i< 1000; i++){
       // cout << bitset<8>(data[i]) << " ";
        // if(data[i] != ne[i]) cout<< (char)data[i];
   // }/
   //cout << "DONE";
   return true;
    //if ((content_size - content_ind) != 0)
    //   cout << "Insufficient space. " << content_ind << " bytes out of " << content_size << " were written.";
}

bool READ(char* data, float a_threshold_header_i, float a_threshold_header_f, float a_threshold_block, int block_height, dataobject * output) {
  //  cout << a_threshold_header_i << " " << a_threshold_header_f << " " << a_threshold_block << " " << block_height << " ";
    PIXEL_ARR_POINTER = char_arr_to_int(&data[10], 4);
  //  cout <<  "\n" << char_arr_to_int(&data[10], 4) << " ";
    WIDTH = char_arr_to_int(&data[18], 4);
    HEIGHT = char_arr_to_int(&data[22], 4);
    BIT_PER_PIXEL = char_arr_to_int(&data[28], 2);
    BYTE_PER_ROW = ceil(((double)WIDTH * ((double)BIT_PER_PIXEL / 8.00))/4.00) * 4;
    BYTE_PER_PIXEL = ((double)BIT_PER_PIXEL/8.0);//
        unsigned int block_height_amount = (int) floor(((double) HEIGHT) / ((double) block_height));
       
        unsigned int block_width_amount = (int) floor(
                (double) WIDTH / 8.00); // width amount of pixels(1 bit per pixel)/8 bits per read block
        unsigned int HeadBlockerSize = ceil(4.00 / (float) block_height);
        char *HB = new char[4];
        // uint8_t * container = new uint8_t[2];
        unsigned int iter_length; //match with byte iter
        unsigned int iter_length2;
        unsigned int iter_length3; //match with block iter
        unsigned int BLOCK_ITER = 0;
        unsigned int BYTE_ITER = 0;
        std::vector<bool> FLIPYN;
        lineqn bleqn(8,a_threshold_header_i,a_threshold_header_f);
        unsigned int HB_COUNT = 0;
        float efe = a_threshold_header_i;
        bool read_headerbytes = false;
        for (int L = 0; L < 8; L++) { //bit significance iteration, up to a noise limit
            //  L = 7;
            //cout << BIT_PER_PIXEL;
            if(HB_COUNT < 5) efe = bleqn.get(L);
            else efe = a_threshold_block;
         //   cout << "EFE: " << efe << "\n";
            for (int k = 0; k < (BIT_PER_PIXEL / 8); k++) { //iteration of selected color spectrum
                 //    cout << "HEE\n";

                for (int i = 0; i < block_height_amount; i++) {
                  //    cout << "HE\n";

                    for (int j = 0; j < block_width_amount; j++) {
                        //      cout << FILEE.fail();
                          //  cout << BYTE_ITER << "\n";
                        //  cout << (read_headerbytes == false ? a_threshold_header : a_threshold_block) << " ";
                        //  uint8_t * tmpdel = container;
                        //  int eeee = 0;
                        uint8_t *container = get_pixel8_block_ARR(block_height, j * 8, i * block_height, k, L, data);
                        // delete[] tmpdel;
                     //   cout << "HI";
                        float v = get_a_factor(container, block_height);
                        //    cout << "\n" << v;
                       //     cout << "\nHB: " << read_headerbytes << " " << HB_COUNT<< "\n";
                        if (read_headerbytes && HB_COUNT < 5) {
                            //   cout << "BLOCK: " << j*8 << "  " << i*block_height << "\n";

                            for (int m = 0; m < block_height; m++) {
                                //     cout << "\n" << bitset<8>(container[m]) << " " << m << "\n";

                                if (HB_COUNT < 4) {
                                    HB[HB_COUNT] = container[m];
                                   //    cout << bitset<8>(container[m]) << " " << j * 8 << " " << i * block_height << "\n";
                                    HB_COUNT++;
                                    if (HB_COUNT == 4) {
                                    //    cout << "\nILID\n";
                                        //HB_COUNT++;
                                        iter_length = char_arr_to_int(HB, 4);
                                        iter_length2 = ceil((float) iter_length / (float) block_height);
                                        iter_length3 = iter_length2;
                                      //        cout << iter_length << " I2: " << iter_length2;
                                        output->arr = new char[iter_length];
                                        output->size = iter_length;
                                       // output.arr = new char[iter_length];
                                       // output.size = iter_length;
                                        efe = a_threshold_block;
                                       // cout << efe;
                                    }
                                } else {
                                    //     cout << "\nHERE";
                                    for (int bs = 0; bs < 8; bs++) {
                                        FLIPYN.emplace_back( (bool)get_bit(container[m], bs));
                                        //   cout << FLIPYN[iter_length3 - iter_length2];
                                        iter_length2--;
                                        if (iter_length2 == 0) {
                                            HB_COUNT++;
                                            //for (int ie = 0; ie < iter_length3; ie++){
                                            //    cout << FLIPYN[ie];
                                            //}
                                            break;
                                        }
                                    }
                                }
                            }
                        } else if (v > efe) { //success: sufficiently noisy to be data block
                            // cout << "HO";
                            if (HB_COUNT < 4 && !read_headerbytes) { read_headerbytes = true; }
                            else {
                                //cout << "HI";
                                if (FLIPYN[BLOCK_ITER]) pattern_convert(container, block_height);
                              //   cout<<" "<< iter_length2 << "e " << BLOCK_ITER;
                                for (int bl = 0; bl < block_height; bl++) {
                                    //        cout << bl;
                                       //    cout << "\nE" << BYTE_ITER << " IE:" << iter_length << " " << bitset<8>(container[bl]);
                                    //cout << (char)container[bl];
                                    output -> arr[BYTE_ITER] = (char)container[bl];
                                   // cout << (char)output->arr[BYTE_ITER];
                                    BYTE_ITER++;
                                    if (BYTE_ITER == iter_length) {//cout << "RT";
                                        delete[] (char*)HB;
                                        delete[] (char*)container;
                                       //cout << (int)output->size;
                                        // cout << string(output -> arr, output -> size);
                                       // cout << "DONEer";
                                        return true;
                                    }
                                    //        cout << "RT";
                                }
                                BLOCK_ITER++;
                            }
                        }
                        delete[] (char*)container;
                        //   cout << "TEST";
                    }
                }
            }
        }
       // cout << "READ ERR";
        return false;
}

///*

void buffer_delete_callback(char* data, void* the_vector) {
  delete[] (char*)the_vector;
}

void buffer_delete_callback2(char* data, void* the_vector) {
    dataobject* out = (dataobject*)the_vector;
    delete[] out->arr;
    delete out;
  }

NAN_METHOD(WriteBMP) {
    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    auto buffer = info[0]->ToObject(context).ToLocalChecked();
    size_t size = node::Buffer::Length(buffer);
    char* output = new char[size];
    memcpy(output, node::Buffer::Data(buffer), size); //I shouldnt need to copy but cant get it working directly off of buffer
   // std::vector<char> insert(node::Buffer::Data(insertdata),node::Buffer::Length(insertdata) + node::Buffer::Data(insertdata));

    auto insertdata = info[1]->ToObject(context).ToLocalChecked();
    size_t idsize = node::Buffer::Length(insertdata);
    std::string::size_type sz;

    int BlockHeight = (info[2]->Uint32Value(context).ToChecked());
    v8::Isolate* isolate = info.GetIsolate(); //there has to be a better way of doing this
    v8::String::Utf8Value str(isolate, info[3]);
    std::string cppStr(*str);
    float Bl_AF_I = stof(cppStr);

    v8::String::Utf8Value str2(isolate, info[4]);
    std::string cppStr2(*str2);
    float Bl_AF_F = stof(cppStr2);

    v8::String::Utf8Value str3(isolate, info[5]);
    std::string cppStr3(*str3);
    float WR_AF = stof(cppStr3);

    if (WRITE(output, node::Buffer::Data(insertdata), idsize, BlockHeight, Bl_AF_I, Bl_AF_F,WR_AF)) {
      //  cout << "\n" << BPR << "\n";

  //  uint8_t * arr = get_pixel8_block_ARR(8, 8, 0, 0, 0, output, 54, 3, BPR);

   // for(int i = 0; i < 8; i++){
  //      cout<<bitset<8>(arr[i]) << "\n";
   // }

    //if (do_convert(output.data(), insert.data(), insert.size(), (int)overwrite_initfac[0], overwrite_initfac[1], overwrite_initfac[2], overwrite_initfac[3])) {
        info.GetReturnValue().Set(
                NewBuffer(output, size, buffer_delete_callback, output).ToLocalChecked()
        );
    }
}
///*
NAN_METHOD(ReadBMP){
 //   cout <<"HI";
        v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
    char*buffer =  (char*) node::Buffer::Data(info[0]->ToObject(context).ToLocalChecked());

    int BlockHeight = (info[1]->Uint32Value(context).ToChecked());

    v8::Isolate* isolate = info.GetIsolate(); //there has to be a better way of doing this
    v8::String::Utf8Value str(isolate, info[2]);
    std::string cppStr(*str);
    float Bl_AF_I = stof(cppStr);

    v8::String::Utf8Value str2(isolate, info[3]);
    std::string cppStr2(*str2);
    float Bl_AF_F = stof(cppStr2);

    v8::String::Utf8Value str3(isolate, info[4]);
    std::string cppStr3(*str3);
    float WR_AF = stof(cppStr3);
    dataobject * output = new dataobject();
    if (READ(buffer, 0.05, 0.65, 0.45, BlockHeight, output)){
        info.GetReturnValue().Set(
            NewBuffer(output->arr, output->size, buffer_delete_callback2, output).ToLocalChecked()
        );
    }

}
//*/
NAN_MODULE_INIT(Init) {

    Nan::Set(target, New<String>("ENCODE").ToLocalChecked(),
             GetFunction(New<FunctionTemplate>(WriteBMP)).ToLocalChecked());

    Nan::Set(target, New<String>("DECODE").ToLocalChecked(),
            GetFunction(New<FunctionTemplate>(ReadBMP)).ToLocalChecked());

}

NODE_MODULE(basic_nan, Init)//*/