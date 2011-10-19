#ifndef ADSCREADER_H
#define ADSCREADER_H


///////////////////////////////////////////////////////////
// YAT::TASK 
///////////////////////////////////////////////////////////
#include <yat/threading/Task.h>
#include <DiffractionImage.h>			//- to read back img data

#define kLO_WATER_MARK      128
#define kHI_WATER_MARK      512

#define kPOST_MSG_TMO       2

#define kTASK_PERIODIC_TIMEOUT_MS    200
const size_t  ADSC_START_MSG     =   (yat::FIRST_USER_MSG + 300);
const size_t  ADSC_STOP_MSG      =   (yat::FIRST_USER_MSG + 301);
const size_t  ADSC_RESET_MSG     =   (yat::FIRST_USER_MSG + 302);

#define TIME_OUT_WATCHER    20*kTASK_PERIODIC_TIMEOUT_MS
///////////////////////////////////////////////////////////


#include <stdlib.h>
#include <limits>

#include "Debug.h"
#include "Data.h"
#include <base.h>
#include <file.h>

#include "HwMaxImageSizeCallback.h"
#include "AdscCamera.h"
#include "HwBufferCtrlObj.h"


using namespace lima;
using namespace lima::Adsc;
using namespace std;


namespace lima
{
namespace Adsc
{

/*******************************************************************
 * \class Reader
 * \brief object involved reading/watching IMG files generated by Adsc
 *******************************************************************/

class Reader : public yat::Task
{
  DEB_CLASS_NAMESPC(DebModCamera, "Reader", "Adsc");

public:

  Reader(Camera& cam, HwBufferCtrlObj& buffer_ctrl);
  virtual ~Reader();

  void start();
  void stop(bool immediatley = true);
  void reset();
  int  getLastAcquiredFrame(void);
  bool isTimeoutSignaled(void);
bool isRunning(void);
  //- [yat::Task implementation]
protected: 
  virtual void handle_message( yat::Message& msg )    throw (yat::Exception);

private:
  void addNewFrame(std::string filename = "");

  //- Mutex
  yat::Mutex                  lock_;
  Camera&                     m_cam;
  HwBufferCtrlObj&            m_buffer;
  bool						  m_use_dw;
  int                         m_image_number;
  int                         m_elapsed_seconds_from_stop;
  int                         m_time_out_watcher;
  bool 						  m_stop_done;
  bool 						  is_running;
  gdshare::DirectoryWatcher*  m_dw;//intended to monitor the arrival of files in a directory
  
  //Loading image stuff!
  Size                        m_image_size;
  DI::DiffractionImage*       m_DI;    

  //simulate an image !
  uint16_t*                   m_image;

};
} // namespace Adsc
} // namespace lima


#endif // ADSCREADER_H
