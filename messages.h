#ifndef TPRK77_MESSAGES_H
#define TPRK77_MESSAGES_H

#include <string.h>

const char PROGMEM tprk77_password[] = "???";

const char PROGMEM tprk77_cyphered_data[] = (
    "1ga\\o\0"
    ";VOjZLVf`O\\m`Ie]lZTkRLUyiL[Og[QLl[NVkaUan_PdZmRPlNIUo[xmNZQiOacTnNV`\\PP_\0"
    "([Llg\\]kQ\\[]Sn_VmVT`_MUh[\\maPM^l_^ZZP`-j\0"
    "2aMYbf`SPWZNlU^a]_Z\0"
    "3_lT[fKLabRZk^\\qQQnQLO\\PQj\\OJUlZ\\^QN\0"
    "4414,(/+ACA-,.9mnm\0"
    "\?_PUT_bjPaiLlUW\\[URgzuyi[ackPjKS]_VYI[RV[\0"
    "APU]leXPTj\\^\0"
    ">aLZ[h[USlOMWRqPXaPHRc\0"
    "DPgJJU[]agXONd]kfT[VY`ajXHRU\0"
    "BUMkWN`ncWZf[YaR^gHhT\\c[\\MS[qQQVTUaiMfjKH[YfW[OkKdShkbXHZVj`_WUNZ\0"
    "=UgYclY^_Wkf/i[\\]K\\YP[Sm\0"
    "CYYa[[o\0"
);

const int PROGMEM tprk77_num_messages = 13;

const int PROGMEM tprk77_message_offsets[] = {
  0, 6, 79, 120, 140, 177, 196, 238, 251, 274, 303, 369, 394
};

/* This will point to the decyphered messages */
const char* tprk77_message_data = 0;

void tprk77_decypher_messages(void)
{
  static char tprk77_message_data_[sizeof(tprk77_cyphered_data)] = {0};
  const int password_length = strlen(tprk77_password);
  for (int ii = 0; ii < sizeof(tprk77_cyphered_data); ++ii) {
    if (tprk77_cyphered_data[ii] == '\0') continue;
    const int password_offset = ii % password_length;
    const int password_char = (tprk77_password[password_offset] - 32) % (127 - 32);
    const int cyphered_char = (tprk77_cyphered_data[ii] - 32) % (127 - 32);
    const int message_char = (cyphered_char + password_char) % (127 - 32) + 32;
    tprk77_message_data_[ii] = message_char;
  }
  tprk77_message_data = tprk77_message_data_;
}

#endif
