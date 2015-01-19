/*
 * Copyright (C) 2014  Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef mozilla_nfcd_NfcTagManager_h
#define mozilla_nfcd_NfcTagManager_h

#include <pthread.h>
#include <vector>

#include "INfcTag.h"
extern "C"
{
  #include "nfa_rw_api.h"
}

class NfcTagManager
  : public INfcTag
{
public:
  NfcTagManager();
  virtual ~NfcTagManager();

  // INfcTag interface.
  bool connect(TagTechnology technology);
  bool disconnect();
  bool reconnect();
  NdefMessage* readNdef();
  NdefInfo* readNdefInfo();
  bool writeNdef(NdefMessage& ndef);
  bool presenceCheck();
  bool makeReadOnly();
  bool formatNdef();
  bool transceive(const std::vector<uint8_t>& command,
                  std::vector<uint8_t>& outResponse);

  std::vector<TagTechnology>& getTechList() { return mTechList; };
  std::vector<int>& getTechHandles() { return mTechHandles; };
  std::vector<int>& getTechLibNfcTypes() { return mTechLibNfcTypes; };
  std::vector<std::vector<uint8_t> >& getTechPollBytes() { return mTechPollBytes; };
  std::vector<std::vector<uint8_t> >& getTechActBytes() { return mTechActBytes; };
  std::vector<uint8_t>& getUid() { return mUid; };
  int& getConnectedHandle() { return mConnectedHandle; };

  /**
   * Does the tag contain a NDEF message?
   *
   * @param  ndefInfo NDEF info.
   * @return          Status code; 0 is success.
   */
  static int doCheckNdef(int ndefInfo[]);

  /**
   * Notify tag I/O operation is timeout.
   *
   * @return None
   */
  static void notifyRfTimeout();

   /**
   * Receive the completion status of transceive operation.
   *
   * @param  buf    Contains tag's response.
   * @param  bufLen Length of buffer.
   * @return None
   */
  static void doTransceiveComplete(uint8_t* buf, uint32_t bufLen);

  /**
   * Send raw data to the tag; receive tag's response.
   *
   * @param  command     Contains command to send.
   * @param  outResponse Contains tag's response.
   * @return True if ok.
   */
  static bool doTransceive(const std::vector<uint8_t>& command,
                           std::vector<uint8_t>& outResponse);

  /**
   * Read the NDEF message on the tag.
   *
   * @param  buf NDEF message read from tag.
   * @return     None.
   */
  static void doRead(std::vector<uint8_t>& buf);

  /**
   * Write a NDEF message to the tag.
   *
   * @param  buf Contains a NDEF message.
   * @return     True if ok.
   */
  static bool doWrite(std::vector<uint8_t>& buf);

  /**
   * Unblock all thread synchronization objects.
   *
   * @return None.
   */
  static void doAbortWaits();

  /**
   * Receive the completion status of read operation. Called by
   * NFA_READ_CPLT_EVT.
   *
   * @param  status Status of operation.
   * @return        None.
   */
  static void doReadCompleted(tNFA_STATUS status);

  /**
   * Receive the completion status of write operation. Called by
   * NFA_WRITE_CPLT_EVT.
   *
   * @param  isWriteOk Status of operation.
   * @return           None.
   */
  static void doWriteStatus(bool isWriteOk);

  /**
   * Receive the completion status of connect operation.
   *
   * @param  isConnectOk Status of the operation.
   * @return             None.
   */
  static void doConnectStatus(bool isConnectOk);

  /**
   * Receive the completion status of deactivate operation.
   *
   * @return None.
   */
  static void doDeactivateStatus(int status);

  /**
   * Connect to the tag in RF field.
   *
   * @param  targetHandle Handle of the tag.
   * @return Must return NXP status code, which NFC service expects.
   */
  static int doConnect(int targetHandle);

  /**
   * Reset variables related to presence-check.
   *
   * @return None.
   */
  static void doResetPresenceCheck();

  /**
   * Receive the result of presence-check.
   *
   * @param  status Result of presence-check.
   * @return        None.
   */
  static void doPresenceCheckResult(tNFA_STATUS status);

  /**
   * Receive the result of checking whether the tag contains a NDEF
   * message. Called by the NFA_NDEF_DETECT_EVT.
   *
   * @param  status      Status of the operation.
   * @param  maxSize     Maximum size of NDEF message.
   * @param  currentSize Current size of NDEF message.
   * @param  flags       Indicate various states.
   * @return             None.
   */
  static void doCheckNdefResult(tNFA_STATUS status, uint32_t maxSize, uint32_t currentSize, uint8_t flags);

  /**
   * Register a callback to receive NDEF message from the tag
   * from the NFA_NDEF_DATA_EVT.
   *
   * @return None.
   */
  static void doRegisterNdefTypeHandler();

  /**
   * No longer need to receive NDEF message from the tag.
   *
   * @return None.
   */
  static void doDeregisterNdefTypeHandler();

  /**
   * Check if the tag is in the RF field.
   *
   * @return None.
   */
  static bool doPresenceCheck();

  /**
   * Deactivate the RF field.
   *
   * @return True if ok.
   */
  static bool doDisconnect();

  /**
   * Receive the result of making a tag read-only. Called by the
   * NFA_SET_TAG_RO_EVT.
   *
   * @param  status Status of the operation.
   * @return        None.
   */
  static void doMakeReadonlyResult(tNFA_STATUS status);

  /**
   * Make the tag read-only.
   *
   * @return True if ok.
   */
  static bool doMakeReadonly();

  /**
   * Receive the completion status of format operation. Called by NFA_FORMAT_CPLT_EVT.
   *
   * @param  isOk Status of operation.
   * @return      None.
   */
  static void formatStatus(bool isOk);

  /**
   * Format a tag so it can store NDEF message.
   *
   * @return True if ok.
   */
  static bool doNdefFormat();

  static bool doIsNdefFormatable();

  int connectWithStatus(int technology);
  int reconnectWithStatus(int technology);
  int reconnectWithStatus();
  NdefMessage* doReadNdef();
  NdefInfo* doReadNdefInfo();
  bool isNdefFormatable();

private:
  pthread_mutex_t mMutex;

  std::vector<TagTechnology> mTechList;
  std::vector<int> mTechHandles;
  std::vector<int> mTechLibNfcTypes;
  std::vector<std::vector<uint8_t> > mTechPollBytes;
  std::vector<std::vector<uint8_t> > mTechActBytes;
  std::vector<uint8_t> mUid;

  // mConnectedHandle stores the *real* libnfc handle
  // that we're connected to.
  int mConnectedHandle;

  // mConnectedTechIndex stores to which technology
  // the upper layer stack is connected. Note that
  // we may be connected to a libnfchandle without being
  // connected to a technology - technology changes
  // may occur runtime, whereas the underlying handle
  // could stay present. Usually all technologies are on the
  // same handle, with the exception of multi-protocol
  // tags.
  int mConnectedTechIndex; // Index in mTechHandles.

  bool mIsPresent; // Whether the tag is known to be still present.

  /**
   * Deactivates the tag and re-selects it with the specified
   * rf interface.
   *
   * @param  rfInterface Type of RF interface.
   * @return             Status code, 0 on success, 1 on failure,
   *                     146 (defined in service) on tag lost.
   */
  static int reSelect(tNFA_INTF_TYPE rfInterface);

  /**
   * Switch controller's RF interface to frame, ISO-DEP, or NFC-DEP.
   *
   * @param  rfInterface Type of RF interface.
   * @return             True if ok.
   */
  static bool switchRfInterface(tNFA_INTF_TYPE rfInterface);

  NdefType getNdefType(int libnfcType);

  int getConnectedLibNfcType();
};

#endif // mozilla_nfcd_NfcTagManager_h
