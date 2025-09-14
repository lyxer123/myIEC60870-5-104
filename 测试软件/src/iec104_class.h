/*
 * This software implements an IEC 60870-5-104 protocol tester.
 * Copyright © 2010-2022 Ricardo L. Olsen
 *
 * Disclaimer
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the
 * Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef IEC104_CLASS_H
#define IEC104_CLASS_H

// IEC 60870-5-104 BASE CLASS, MASTER IMPLEMENTATION

#include "iec104_types.h"
#include "logmsg.h"
#include <map>
#include <string>

#pragma pack(push)
#pragma pack(1) // byte aligned structures

struct iec_obj {
  uint32_t address; // 3 bytes address in 4 bytes space

  double value; // value 8 bytes float

  cp56time2a timetag;      // 7 bytes time tag
  cp16Time2a elapsed_time; // 2 byte elapsed time in ms

  uint8_t type;  // iec ASDU type
  uint8_t cause; // COT cause of transmission
  uint16_t ca;   // common address of asdu

  union {
    uint8_t ov : 1;  // overflow/no overflow
    uint8_t sp : 1;  // single point information
    uint8_t dp : 2;  // double point information
    uint8_t scs : 1; // single command state
    uint8_t dcs : 2; // double command state
    uint8_t rcs : 2; // regulating step command
  };                 // 2 bits

  uint8_t qu : 5; // qualifier of command
  uint8_t se : 1; // select=1 / execute=0
  // + 6 bits = 8 bits

  uint8_t t : 1;    // transient flag
  uint8_t bl : 1;   // blocked/not blocked
  uint8_t sb : 1;   // substituted/not substituted
  uint8_t nt : 1;   // not topical/topical
  uint8_t iv : 1;   // valid/invalid
  uint8_t pn : 1;   // 0=positive, 1=negative
  uint8_t cy : 1;   // counter carry
  uint8_t cadj : 1; // counter adjust
  // 8 bits=1 byte

  union {
    iec_stcd stcd;   // 32 bit 16 status and 16 status change detection
    iec_bsi bsi;     // binary state information 4 bytes unsigned structured
    uint32_t bcr;    // binary counter reading 4 bytes unsigned
    iec_spe spe;     // start event of protection
    iec_oci oci;     // output circuit information
    uint8_t qpa;     // qualifier of parameter activation
    uint8_t kpa : 6; // kind of parameter
    uint8_t frz : 2; // freeze/reset qualifier of counter interrogation command
  };
  uint8_t sq : 5;   // counter seq
  uint8_t pop : 1;  // pop
  uint8_t lpc : 1;  // lpc
  uint8_t ei : 1;   // elapsed invalid
  uint8_t test : 1; // test bit
};

#pragma pack(pop)

class iec104_class {
public:
  static const unsigned int M_SP_NA_1 = 1;  // single-point information
  static const unsigned int M_DP_NA_1 = 3;  // double-point information
  static const unsigned int M_ST_NA_1 = 5;  // step position information
  static const unsigned int M_BO_NA_1 = 7;  // bitstring of 32 bits
  static const unsigned int M_ME_NA_1 = 9;  // normalized value
  static const unsigned int M_ME_NB_1 = 11; // scaled value
  static const unsigned int M_ME_NC_1 = 13; // floating point
  static const unsigned int M_IT_NA_1 = 15; // integrated totals
  static const unsigned int M_PS_NA_1 =
      20; // packed single point information with status change detection
  static const unsigned int M_ME_ND_1 =
      21; // normalized value without quality descriptor
  static const unsigned int M_SP_TB_1 =
      30; // single-point information with time tag
  static const unsigned int M_DP_TB_1 =
      31; // double-point information with time tag
  static const unsigned int M_ST_TB_1 =
      32; // step position information with time tag
  static const unsigned int M_BO_TB_1 =
      33; // bitstring of 32 bits with time tag
  static const unsigned int M_ME_TD_1 = 34; // normalized value with time tag
  static const unsigned int M_ME_TE_1 = 35; // scaled value with time tag
  static const unsigned int M_ME_TF_1 = 36; // floating point with time tag
  static const unsigned int M_IT_TB_1 = 37; // integrated totals with time tag
  static const unsigned int M_EP_TD_1 =
      38; // event of protection equipment with CP56Time2a time tag
  static const unsigned int M_EP_TE_1 =
      39; // packed start events of protection equipment with CP56Time2a time
          // tag
  static const unsigned int M_EP_TF_1 =
      40; // packed output circuit information of protection equipment with
          // CP56Time2a time tag
  static const unsigned int C_SC_NA_1 = 45; // single command
  static const unsigned int C_DC_NA_1 = 46; // double command
  static const unsigned int C_RC_NA_1 = 47; // regulating step command
  static const unsigned int C_SE_NA_1 = 48; // set-point normalised command
  static const unsigned int C_SE_NB_1 = 49; // set-point scaled command
  static const unsigned int C_SE_NC_1 =
      50; // set-point short floating point command
  static const unsigned int C_BO_NA_1 = 51; // bitstring of 32 bit command
  static const unsigned int C_SC_TA_1 = 58; // single command with time tag
  static const unsigned int C_DC_TA_1 = 59; // double command with time tag
  static const unsigned int C_RC_TA_1 =
      60; // regulating step command with time tag
  static const unsigned int C_SE_TA_1 =
      61; // set-point normalised command with time tag
  static const unsigned int C_SE_TB_1 =
      62; // set-point scaled command with time tag
  static const unsigned int C_SE_TC_1 =
      63; // set-point short floating point command with time tag
  static const unsigned int C_BO_TA_1 =
      64; // bitstring of 32 bit command with time tag
  static const unsigned int M_EI_NA_1 = 70;  // end of initialization
  static const unsigned int C_IC_NA_1 = 100; // general interrogation (GI)
  static const unsigned int C_CI_NA_1 = 101; // counter interrogation
  static const unsigned int C_RD_NA_1 = 102; // read command
  static const unsigned int C_CS_NA_1 = 103; // clock synchronization command
  static const unsigned int C_RP_NA_1 = 105; // reset process command
  static const unsigned int C_TS_TA_1 =
      107; // test command with time tag CP56Time2a
  static const unsigned int P_ME_NA_1 =
      110; // Parameter of measured values, normalized value
  static const unsigned int P_ME_NB_1 =
      111; // Parameter of measured values, scaled value
  static const unsigned int P_ME_NC_1 =
      112; // Parameter of measured values, short floating point number
  static const unsigned int P_AC_NA_1 = 113; // Parameter activation

  /* cause of transmition (standard) */
  static const unsigned int CYCLIC = 1;
  static const unsigned int BGSCAN = 2;
  static const unsigned int SPONTANEOUS = 3;
  static const unsigned int REQUEST = 5;
  static const unsigned int ACTIVATION = 6;
  static const unsigned int ACTCONFIRM = 7;
  static const unsigned int DEACTIVATION = 8;
  static const unsigned int ACTTERM = 10;

  static const unsigned int SUPERVISORY = 0x01;
  static const unsigned int STARTDTACT = 0x07;
  static const unsigned int STARTDTCON = 0x0B;
  static const unsigned int STOPDTACT = 0x13;
  static const unsigned int STOPDTCON = 0x23;
  static const unsigned int TESTFRACT = 0x43;
  static const unsigned int TESTFRCON = 0x83;
  static const unsigned int INTERROGATION = 0x64;
  static const unsigned int START = 0x68;
  static const unsigned int RESET = 0x69;

  static const unsigned int POSITIVE = 0;
  static const unsigned int NEGATIVE = 1;

  static const unsigned int SELECT = 1;
  static const unsigned int EXECUTE = 0;

  TLogMsg mLog;

  // ---- user called funcions, must be called by the user -----------------
  iec104_class();         // user called constructor on derived class
  void onConnectTCP();    // user called, when tcp connected
  void onDisconnectTCP(); // user called, when tcp disconnected
  void onTimerSecond();   // user called, each second timer
  void packetReadyTCP();  // user called, when packet ready to be read from tcp
                          // connection

  void solicitGI();                           // General Interrogation
  void solicitInterrogation(char group = 20); // Group interrogation
  void setSecondaryIP(char *ip);
  void setSecondaryIP_backup(char *ip);
  char *getSecondaryIP();
  char *getSecondaryIP_backup();
  void setSecondaryASDUAddress(int addr);
  void setSecondaryAddress(int addr);
  int getSecondaryAddress();
  void setPrimaryAddress(int addr);
  int getPrimaryAddress();
  void disableSequenceOrderCheck(); // allow sequence out of order
  bool sendCommand(iec_obj *obj);   // Command, return false if not send
  int getPortTCP();
  void setPortTCP(unsigned port);
  void setGIPeriod(unsigned period);
  std::map<int, std::string> mapTiStr;
  std::map<int, std::string> mapCauseStr;
  std::string asduTiStr(int ti);
  std::string causeStr(int cause);

private:
  unsigned short VS;      // sender packet control counter
  unsigned short VR;      // receiver packet control counter
  void confTestCommand(); // test command activation confirmation
  void sendStartDTACT();  // send STARTDTACT
  int tout_startdtact;    // timeout control
  void sendSupervisory(); // send supervisory window control frame
  int tout_supervisory;   // countdown to send supervisory window control
  int tout_gi;            // countdown to send general interrogation
  int tout_testfr;        // countdown to send test frame
  bool broken_msg = false;
  bool connectedTCP; // tcp connection state
  bool
      seq_order_check; // if set: test message order, disconnect if out of order
  unsigned char masterAddress; // master link address (primary address,
                               // originator address, oa)
  unsigned short slaveAddress; // slave link address (secondary address, common
                               // address of ASDU, ca)
  unsigned short slaveASDUAddrCmd; // common address of ASDU, ca for commands
  unsigned Port;                   // iec104 tcp port (defaults to 2404)
  char slaveIP[21];                // slave (secondary, main RTU) IP address
  char slaveIP_backup[21];         // slave (secondary, backup RTU) IP address
  static const int t3_testfr = 10;
  static const int t2_supervisory = 8;
  static const int t1_startdtact = 6;
  int gi_period; // minimum time for request between GI's
  static const int gi_retry_time =
      45; // wait time to retry when requested a GI and not responded
  unsigned short test_command_count = 0; // test command counter
  char logPointBuf[15000] = "     ";

protected:
  void LogFrame(char *frame, int size, bool is_send);
  void LogPoint(char *buf, int address, double val, char *qualifier,
                cp56time2a *timetag);
  void parseAPDU(iec_apdu *papdu, int sz,
                 bool accountandrespond =
                     true); // parse APDU, ( accountandrespond == false :
                            // process the apdu out of the normal handshake )
  char *trim(char *s);

  int msg_supervisory;

  bool TxOk;            // ready to transmit state (STARTDTCON received)
  unsigned GIObjectCnt; // contador de objetos da GI

  // ---- pure virtual funcions, user defined on derived class (mandatory)---

  // wait milliseconds for data bytes
  virtual void waitBytes(int bytes, int msTout) = 0;
  // make tcp connection, user provided
  virtual void connectTCP() = 0;
  // tcp disconnect, user provided
  virtual void disconnectTCP() = 0;
  // read tcp data, user provided
  virtual int readTCP(char *buf, int szmax) = 0;
  // send tcp data, user provided
  virtual void sendTCP(char *data, int sz) = 0;
  // check bytes available in tcp connection
  virtual int bytesAvailableTCP() = 0;

  // ---- virtual funcions, user defined on derived class (not mandatory)---

  // user point process, user provided. (on one call must be only objects of one
  // type)
  virtual void dataIndication(iec_obj * /*obj*/, unsigned /*numpoints*/) {}
  // inform user that ACTCONFIRM of Interrogation was received from slave
  virtual void interrogationActConfIndication() {}
  // inform user that ACTTERM of Interrogation was received from slave
  virtual void interrogationActTermIndication() {}
  // inform user of command activation
  virtual void commandActRespIndication(iec_obj * /*obj*/) {}
  // user process APDU
  virtual void userprocAPDU(iec_apdu * /* papdu */, int /* sz */) {}

  // -------------------------------------------------------------------------
};

#endif // IEC104_CLASS_H
