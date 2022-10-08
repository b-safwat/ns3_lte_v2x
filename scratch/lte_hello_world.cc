/*
 * lte_hello_world.cc
 *
 * url: https://www.nsnam.org/docs/models/html/lte-user.html
 *
 *  Created on: Oct 6, 2022
 *      Author: bassel
 */


#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>

using namespace ns3;

int main (int argc, char *argv[])
{
	// the rest of the simulation program follows
	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();

	// There are several types of LTE MAC scheduler user can choose here.
	// User can use following codes to define scheduler type:
	lteHelper->SetSchedulerType ("ns3::FdMtFfMacScheduler");    // FD-MT scheduler
	lteHelper->SetSchedulerType ("ns3::TdMtFfMacScheduler");    // TD-MT scheduler
	lteHelper->SetSchedulerType ("ns3::TtaFfMacScheduler");     // TTA scheduler
	lteHelper->SetSchedulerType ("ns3::FdBetFfMacScheduler");   // FD-BET scheduler
	lteHelper->SetSchedulerType ("ns3::TdBetFfMacScheduler");   // TD-BET scheduler
	lteHelper->SetSchedulerType ("ns3::FdTbfqFfMacScheduler");  // FD-TBFQ scheduler
	lteHelper->SetSchedulerType ("ns3::TdTbfqFfMacScheduler");  // TD-TBFQ scheduler
	// lteHelper->SetSchedulerAttribute("DebtLimit", IntegerValue(yourvalue)); // default value -625000 bytes (-5Mb)
	// lteHelper->SetSchedulerAttribute("CreditLimit", UintegerValue(yourvalue)); // default value 625000 bytes (5Mb)
	// lteHelper->SetSchedulerAttribute("TokenPoolSize", UintegerValue(yourvalue)); // default value 1 byte
	// lteHelper->SetSchedulerAttribute("CreditableThreshold", UintegerValue(yourvalue)); // default value 0

	lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler");     //PSS scheduler
	// lteHelper->SetSchedulerAttribute("nMux", UIntegerValue(yourvalue)); // the maximum number of UE selected by TD scheduler
	// lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA")); // PF scheduler type in PSS

	// Create Node objects for the eNB(s) and the UEs:
	NodeContainer enbNodes;
	enbNodes.Create (1);
	NodeContainer ueNodes;
	ueNodes.Create (2);

	// Configure the Mobility model for all the nodes:
	MobilityHelper mobility;
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (enbNodes);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (ueNodes);

	// Install an LTE protocol stack on the eNB(s):

	NetDeviceContainer enbDevs;
	enbDevs = lteHelper->InstallEnbDevice (enbNodes);

	// Install an LTE protocol stack on the UEs:
	NetDeviceContainer ueDevs;
	ueDevs = lteHelper->InstallUeDevice (ueNodes);

	// Attach the UEs to an eNB. This will configure each UE according to the eNB configuration,
	// and create an RRC connection between them:
	/*
	 * The Radio Resource Control (RRC) protocol is used in UMTS, LTE and 5G on the Air interface.
	 *  It is a layer 3 (Network Layer) protocol used between UE and Base Station.
	 *  This protocol is specified by 3GPP in TS 25.331[2] for UMTS, in TS 36.331 [3] for LTE
	 *  and in TS 38.331[4] for 5G New Radio. RRC messages are transported via the PDCP-Protocol.
	 *
	 *  The major functions of the RRC protocol include connection establishment and release functions,
	 *  broadcast of system information, radio bearer establishment, reconfiguration and release,
	 *  RRC connection mobility procedures, paging notification and release and outer loop power control.[5]
	 *  By means of the signalling functions the RRC configures the user and control planes according to the network status
	 *  and allows for Radio Resource Management strategies to be implemented.[6]
	 *  	 *
	 * */
	lteHelper->Attach (ueDevs, enbDevs.Get (0));


	// Activate a data radio bearer between each UE and the eNB it is attached to:
	enum EpsBearer::Qci q = EpsBearer::GBR_CONV_VOICE;
	EpsBearer bearer (q);
	lteHelper->ActivateDataRadioBearer (ueDevs, bearer);

	//this method will also activate two saturation traffic generators for that bearer, one in uplink and one in downlink.

	// This is needed otherwise the simulation will last forever, because (among others) the start-of-subframe event is scheduled repeatedly, and the ns-3 simulator scheduler will hence never run out of events.
	Simulator::Stop (Seconds (0.005));

	// Run the simulation:
	Simulator::Run ();

	Simulator::Destroy ();
	return 0;
}


