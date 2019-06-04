#ifndef TPRK77_MESSAGES_H
#define TPRK77_MESSAGES_H

#include <string.h>

const char PROGMEM tprk77_password[] = "???";

const char PROGMEM tprk77_cyphered_data[] = (
    "1ga\\o\0"
    "(ZVjULQStfXW[OkYSqcTTjS]]P_^h\0"
    "*YRmfWaiTaXZVj[]iWV^\\LWfa]l\\TKaq\\[]VLg)k\0"
    "--,.(./.>B@274>okg\0"
    ",Pa_MYhHlRVIYY[VncX_OmKjSWI^h[UO[gMi]WQN[PfeS_UZ][\0"
    "*_c`Pkc\\gakTYL[SP`vgZNLlbUMYiQdXbTYfPOQ\\]MfbVbylIZNlf^kWPH`i_UPgSJTR\\aI`S\\`n[Uj[VOjdZULW\0"
    "1]lIkLNdaQ[jY]VWm\0"
    ",Vh`\\clOQ^lf^kcSLm-W\\`Lf-P`b_QO^lhT^hjVT^P[*\0"
    ",_L_gl[QM\\`SkRZ\\\\^^\0"
    "1=:h9<m1)4j\0"
    "94@n\?6m>32j+/87=/m\0"
    "425GA>A84;7\0"
    "5Parh`\\czgEYbxaQnQP\\KWYdgH`HXSzgEYbqfQaPfb\\dVYOf]VlQ_W_]lfWQnMV`NP_vgYRNUb,\0"
    "4YdquN^`[mKjOTOf0V_][gT_T}nNaZ[VO],k\0"
    "/^O,mDPMvyrnAWwVVjj.jK\\\\[\\[RZl\0"
    "6qd_TOfbYjOPgHWfNRcMZ^bdT^nWPYOjfZ]rh[US[g5iaa^WnLUmK]_Z_fRUlbUMkU[WT\0"
    "8qSZiYRaMYhM\\`TM`idZP`nSH^ZP[PLfXUlbUI`iQShwn^LdOYldMH[ZlOTWl\0"
    "6XnUcjIZOPQ^sf`LlQNVkUV^[kX_\0"
    "7PjdPgKXUsblSZYdq[UUPrmRZdjKHWfdSlSZYdqSQP_O-\0"
    "4ajQZhIRbaM^iaanNTjMSK]ROg[QH[mYWbOQ\0"
    "9QTef:KbRPbgh>UOagU]lfWMc*g\0"
    "4``_gZQV\\blQ`i3agl\0"
    "7PTOjV^gHhQ\\c_VQcxq][cjHmNP`_QUJ[V][\0"
    "9K[VP`dX&m-S\\[XL[g\0"
    ";bZPO_ n.PYKW^]fxg(\\ZNcY\\yiAZTRcxf/XOlWWSU`TOTOUXT \0"
    ":TaL`iRVaMf^WmmAZa]aqh[d]fWX^aTVJ]Zm\0"
    "<PkWfqV[bSgm+j]ZZJ^WV\\Rh\0"
    "9[^hkcSLmaT`P[[hH[Rl[`_][SQb_f]PjZPVfWLcS_gORN`VQ\0"
    ":\\`iR_PI[NZamTT[\\fqX_nYVbiT[jVL_L_mSIXVV`VwnM\\biT[jZP\\P[UlMbO_kn`XXLmaPlQISU\0"
    "\?OVLUXTq_Ma_`n\0"
    "=RLTfTU\\eYMPQRqX_n_VmUY\\bg[QLlSe\\QXaq^RnZUSp^lTOUXYN\\PM\0"
    "=aSa`n_OSiRNXMfJS_SNLej\0"
    "FWQnXH\\ibUZgTX]RalIkW\\g]`PTUmLPTTVZhIfmPI^\\f[]SnL^Ocj`XISUf`b\\VQ]\0"
    "FWQn_P[OjUL[fLVZSl\\[iaWb`n_OgiN\\`ZHPLz{z\0"
    "@RRkn_PdfbRLaj\\ONYRmV[kX\\q\\QSTJWXPl_PH]fPO[gO__WnMnQV]Vxzxg0hNbS`[k^UScrbj[`_P\0"
    "\?PP\\f`WQMk_]\0"
    "FQnNH\\XZajTLJY[mdQ`R\\gck_LP\\\0"
    "BUL\\fRUl]OTU`Va]kX^fbRLak\'\0"
    "@VY]YWl\0"
    "FadrePf[O_lbQ[QfNmaM^\\VT[QnQHbOvlSI]NUsbla[_,\0"
);

const int PROGMEM tprk77_num_messages = 40;

const int PROGMEM tprk77_message_offsets[] = {
  0, 6, 36, 77, 96, 147, 236, 254, 299, 319, 331, 350, 362, 438, 475, 506, 576, 638, 667, 713, 750,
  778, 797, 834, 853, 905, 942, 967, 1017, 1094, 1109, 1165, 1189, 1255, 1296, 1375, 1388, 1417,
  1444, 1452
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
