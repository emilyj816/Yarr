#ifndef NETIOTXCORE_H
#define NETIOTXCORE_H

/********************************
 * NetioTxCore
 * Author: Carlos.Solans@cern.ch
 * Author: Roland.Sipos@cern.ch
 * Description: NetIO Transmitter
 * Date: June 2017
 *********************************/

#include "TxCore.h"

#include "netio/netio.hpp"
#include "felixbase/client.hpp"

#include <cstdint>
#include <queue>
#include <vector>
#include <mutex>

#include "json.hpp"

using json=nlohmann::basic_json<std::map, std::vector, std::string, bool, std::int32_t, std::uint32_t, float>;

/**
 * @brief Implementation of the RxCore for NetIO communication.
 *        This class subscribes to felix using a low_latency_socket.
 **/
class NetioTxCore : virtual public TxCore {
public:
		
  /**
   * @brief Default constructor
   * Create NetIO context and low_latency_send_socket
   **/
  NetioTxCore();

  /**
   * @brief Destructor
   * Join the trigger thread. Disconnect the socket.
   * Delete socket and context.
   **/
  ~NetioTxCore();

  /**
   * @brief append to fifo of all channels
   * @param value command as uint32_t
   **/
  void writeFifo(uint32_t value) override;

  /**
   * @brief release the fifo for all enabled channels
   **/
  void releaseFifo() override;

  void setCmdEnable(uint32_t) override;
  uint32_t getCmdEnable() override;

  /**
   * @brief check if the fifo of commands is empty
   * @return bool true if fifo is empty
   **/
  bool isCmdEmpty() override;

  /**
   * @brief enable the trigger
   * @param value for the trigger (0x0,0x1)
   **/
  void setTrigEnable(uint32_t value) override;

  /**
   * @brief get if the trigger is enabled
   * @return uint32_t get trigger enable
   **/
  uint32_t getTrigEnable() override;

  void maskTrigEnable(uint32_t value, uint32_t mask) override;

  /**
   * @brief check if the trigger is done
   * @return bool is trigger done
   **/
  bool isTrigDone() override;

  /**
   * @brief set the trigger config
   * @param cfg TRIG_CONF_VALUE config value
   **/
  void setTrigConfig(enum TRIG_CONF_VALUE cfg) override;

  /**
   * @brief set the trigger frequency
   * @param freq double frequency in Hz
   **/
  void setTrigFreq(double freq) override;

  /**
   * @brief set the number of desired triggers
   * @param count number of desired triggers
   **/
  void setTrigCnt(uint32_t count) override;

  /**
   * @brief set the trigger time in seconds
   * @param time in seconds
   **/
  void setTrigTime(double time) override;

  /**
   * @brief set Trigger Word Length
   * @param length of the trigger word starting at MSB
   **/
  void setTrigWordLength(uint32_t length) override;

  /**
   * @brief set the trigger words
   * @param words uint32_t pointer to trigger words
   * @param size number of words in the pointer
   **/
  void setTrigWord(uint32_t *words, uint32_t size) override;

  /**
   * @brief abort the trigger sequence
   **/
  void toggleTrigAbort() override;

  /**
   * @brief set the trigger logic
   * @param mask
   **/
  void setTriggerLogicMask(uint32_t mask) override;

  /**
   * @brief set the trigger logic mode
   * @param mode TRIG_LOGIC_MODE_VALUE trigger logic mode
   **/
  void setTriggerLogicMode(enum TRIG_LOGIC_MODE_VALUE mode) override;

  /**
   * @brief reset the trigger logic
   **/
  void resetTriggerLogic() override;

  /**
   * @brief get the number of triggers in
   * @return uint32_t get number of triggers
   **/
  uint32_t getTrigInCount() override;

  /**
   * @brief read configuration from json
   * @param j reference to json
   **/
  void fromFileJson(json& j);

  /**
   * @brief write configuration to json
   * @param j reference to json
   **/
  void toFileJson(json& j);

private:

  enum TRIG_CONF_VALUE m_trigCfg;            //! trigger config
  bool m_verbose;                            //! verbose mode
  bool m_trigEnabled;                        //! trigger is enabled
  uint32_t m_trigMode;                       //! trigger mode
  uint32_t m_trigMask;                       //! trigger mask
  uint32_t m_enableMask;                     //! enable mask
  uint32_t m_trigCnt;                        //! number of triggers
  uint32_t m_trigTime;                       //! trigger time
  uint32_t m_trigFreq;                       //! trigger frequency
  uint32_t m_trigWordLength;                 //! number of trigger words
  std::vector<uint32_t> m_trigWords;         //! the trigger words
  std::map<uint64_t, std::vector<uint8_t> > m_trigFifo;  //! fifo per elink
  std::map<uint64_t, std::vector<uint8_t> > m_fifo;  //! fifo per elink
  std::vector<const uint8_t*> m_data;        //! buffer of addresses for netio
  std::vector<size_t> m_size;                //! buffer of sizes for netio
  std::map<uint64_t,bool> m_elinks;          //! enabled elinks
  std::map<uint64_t,bool> m_trigElinks;       //! trigger elinks
  std::map<netio::tag,felix::base::ToFELIXHeader> m_headers; //! buffer of headers for netio
  netio::low_latency_send_socket * m_socket; //! netio send socket
  netio::context * m_context;                //! netio underlaying technology
  std::thread m_trigProc;                    //! trigger thread
  std::mutex m_mutex;

  void connect();
  void trigger();
  void doTriggerCnt();                       //! loop for a fixed number of triggers
  void doTriggerTime();                      //! loop to trigger during a time slot
  void printFifo(uint64_t elink);
  uint32_t m_extend;
  bool m_padding;
  bool m_flip;
  bool m_manchester;
  bool m_debug;

  void enableChannel(uint64_t channel);
  void disableChannel(uint64_t channel);
  void writeFifo(uint32_t channel, uint32_t value);

  void prepareTrigger();
  void prepareFifo(std::vector<uint8_t> *fifo);
  void writeFifo(std::vector<uint8_t> *fifo, uint32_t value);

  std::string m_felixhost;
  uint16_t m_felixport;
  bool m_connected;

};

#endif
