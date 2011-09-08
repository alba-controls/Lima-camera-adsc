//###########################################################################
// This file is part of LImA, a Library for Image Acquisition
//
// Copyright (C) : 2009-2011
// European Synchrotron Radiation Facility
// BP 220, Grenoble 38043
// FRANCE
//
// This is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//###########################################################################
#include "AdscInterface.h"

/*******************************************************************
 * \brief DetInfoCtrlObj constructor
 *******************************************************************/
DetInfoCtrlObj::DetInfoCtrlObj(Camera& adsc)
	: m_adsc(adsc)
{
}

DetInfoCtrlObj::~DetInfoCtrlObj()
{
}

void DetInfoCtrlObj::getMaxImageSize(Size& max_image_size)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	max_image_size = fdim.getSize();
}

void DetInfoCtrlObj::getDetectorImageSize(Size& det_image_size)
{
	m_adsc.getMaxImageSize(det_image_size);
}

void DetInfoCtrlObj::getDefImageType(ImageType& def_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	def_image_type = fdim.getImageType();
}

void DetInfoCtrlObj::setCurrImageType(ImageType curr_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	fdim.setImageType(curr_image_type);
	m_adsc.setFrameDim(fdim);
}

void DetInfoCtrlObj::getCurrImageType(ImageType& curr_image_type)
{
	FrameDim fdim;
	m_adsc.getFrameDim(fdim);
	curr_image_type = fdim.getImageType();
}

void DetInfoCtrlObj::getPixelSize(double& pixel_size)
{
	pixel_size = 0.1025880;
}

void DetInfoCtrlObj::getDetectorType(string& det_type)
{
	det_type = "Adsc";
}

void DetInfoCtrlObj::getDetectorModel(string& det_model)
{
	det_model = "Q315r";
}

void DetInfoCtrlObj::registerMaxImageSizeCallback(
						HwMaxImageSizeCallback& cb)
{
	m_mis_cb_gen.registerMaxImageSizeCallback(cb);
}

void DetInfoCtrlObj::unregisterMaxImageSizeCallback(
						HwMaxImageSizeCallback& cb)
{
	m_mis_cb_gen.unregisterMaxImageSizeCallback(cb);
}


void DetInfoCtrlObj::
     MaxImageSizeCallbackGen::setMaxImageSizeCallbackActive(bool cb_active)
{
}

/*******************************************************************
 * \brief SyncCtrlObj constructor
 *******************************************************************/
SyncCtrlObj::SyncCtrlObj(Camera& adsc)
	: HwSyncCtrlObj(), m_adsc(adsc)
{
}

SyncCtrlObj::~SyncCtrlObj()
{
}

bool SyncCtrlObj::checkTrigMode(TrigMode trig_mode)
{
	return (trig_mode == ExtTrigSingle);
}

void SyncCtrlObj::setTrigMode(TrigMode trig_mode)
{
	if (!checkTrigMode(trig_mode))
		throw LIMA_HW_EXC(InvalidValue, "Invalid (external) trigger");
}

void SyncCtrlObj::getTrigMode(TrigMode& trig_mode)
{
	trig_mode = ExtTrigSingle;
}

void SyncCtrlObj::setExpTime(double exp_time)
{
	m_adsc.setExpTime(exp_time);
}

void SyncCtrlObj::getExpTime(double& exp_time)
{
	m_adsc.getExpTime(exp_time);
}

void SyncCtrlObj::setLatTime(double lat_time)
{
	m_adsc.setLatTime(lat_time);
}

void SyncCtrlObj::getLatTime(double& lat_time)
{
	m_adsc.getLatTime(lat_time);
}

void SyncCtrlObj::setNbHwFrames(int nb_frames)
{
	m_adsc.setNbFrames(nb_frames);
}

void SyncCtrlObj::getNbHwFrames(int& nb_frames)
{
	m_adsc.getNbFrames(nb_frames);
}

void SyncCtrlObj::getValidRanges(ValidRangesType& valid_ranges)
{
	double min_time = 10e-9;
	double max_time = 1e6;
	valid_ranges.min_exp_time = min_time;
	valid_ranges.max_exp_time = max_time;
	valid_ranges.min_lat_time = min_time;
	valid_ranges.max_lat_time = max_time;
}


/*******************************************************************
 * \brief BinCtrlObj constructor
 *******************************************************************/
BinCtrlObj::BinCtrlObj(Camera& adsc)
	: m_adsc(adsc)
{
}

BinCtrlObj::~BinCtrlObj()
{
}

void BinCtrlObj::setBin(const Bin& bin)
{
	m_adsc.setBin(bin);
}

void BinCtrlObj::getBin(Bin& bin)
{
	m_adsc.getBin(bin);
}

void BinCtrlObj::checkBin(Bin& bin)
{
	m_adsc.checkBin(bin);
}


/*******************************************************************
 * \brief Interface constructor
 *******************************************************************/
Interface::Interface(Camera& adsc)
	: m_adsc(adsc), m_det_info(adsc),
	  m_sync(adsc), m_bin(adsc)
{
	HwDetInfoCtrlObj *det_info = &m_det_info;
	m_cap_list.push_back(HwCap(det_info));

	m_cap_list.push_back(HwCap(adsc.getBufferMgr()));

	HwSyncCtrlObj *sync = &m_sync;
	m_cap_list.push_back(HwCap(sync));

	HwBinCtrlObj *bin = &m_bin;
	m_cap_list.push_back(HwCap(bin));
}

Interface::~Interface()
{
}

void Interface::getCapList(HwInterface::CapList &aReturnCapList) const
{
  aReturnCapList = m_cap_list;
}

void Interface::reset(ResetLevel reset_level)
{
	m_adsc.reset();
}

void Interface::prepareAcq()
{
}

void Interface::startAcq()
{
	m_adsc.startAcq();
}

void Interface::stopAcq()
{
	m_adsc.stopAcq();
}

void Interface::getStatus(StatusType& status)
{
	Camera::Status adsc_status = m_adsc.getStatus();
	switch (adsc_status) {
	case Camera::Ready:
		status.acq = AcqReady;
		status.det = DetIdle;
		break;
	case Camera::Exposure:
		status.det = DetExposure;
		goto Running;
	case Camera::Readout:
		status.det = DetReadout;
		goto Running;
	case Camera::Latency:
		status.det = DetLatency;
	Running:
		status.acq = AcqRunning;
		break;
	}
	status.det_mask = DetExposure | DetReadout | DetLatency;
}

int Interface::getNbHwAcquiredFrames()
{
	return m_adsc.getNbAcquiredFrames();
}

