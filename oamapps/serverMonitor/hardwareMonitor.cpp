/***************************************************************************
 * $Id: hardwareMonitor.cpp 34 2006-09-29 21:13:54Z dhill $
 *
 *   Author: David Hill
 ***************************************************************************/

#include "serverMonitor.h"

using namespace std;
using namespace oam;
using namespace snmpmanager;
using namespace logging;
using namespace servermonitor;
//using namespace procheartbeat;


/************************************************************************************************************
* @brief	hardwareMonitor function
*
* purpose:	Monitor Hardware and report problems
*
* Parses file generated by the ipmitool
*
* pattern =  what it is | value | units | status | value 1 | value 2 | value 3 | value 4 | value 5 | value 6
* data(0) = what it is
* data(1) = value
* data(2) = units
* data(3) = status
* data(4)-data(9) = barrier values
*   data(4) - low non-recoverable, i.e. fatal
*   data(5) - low critical
*   data(6) - low warning
*   data(7) - high warning
*   data(8) - high critical
*   data(9) - high non-recoverable, i.e. fatal
*
************************************************************************************************************/

void hardwareMonitor(int IPMI_SUPPORT)
{
	ServerMonitor serverMonitor;
	string data[10];
	string SensorName;
	float SensorValue;
    string Units;
	string SensorStatus;
	float lowFatal;
	float lowCritical;
	float lowWarning;
	float highWarning;
	float highCritical;
	float highFatal;
	char *p;

	if( IPMI_SUPPORT == 0) {
		int returnCode = system("ipmitool sensor list > /tmp/harwareMonitor.txt");
		if (returnCode) {
			// System error, Log this event 
			LoggingID lid(SERVER_MONITOR_LOG_ID);
			MessageLog ml(lid);
			Message msg;
			Message::Args args;
			args.add("Error running ipmitool sensor list!!!");
			msg.format(args);
			ml.logWarningMessage(msg);
			while(TRUE)
				sleep(10000);
		}
	}
	else
	{
		while(TRUE)
			sleep(10000);
	}

	// register for Heartbeat monitoring
/*	try {
		ProcHeartbeat procheartbeat;
		procheartbeat.registerHeartbeat(HW_HEARTBEAT_ID);
	}
	catch (exception& ex)
	{
		string error = ex.what();
		LoggingID lid(SERVER_MONITOR_LOG_ID);
		MessageLog ml(lid);
		Message msg;
		Message::Args args;
		args.add("EXCEPTION ERROR on registerHeartbeat: ");
		args.add(error);
		msg.format(args);
		ml.logErrorMessage(msg);
	}
	catch(...)
	{
		LoggingID lid(SERVER_MONITOR_LOG_ID);
		MessageLog ml(lid);
		Message msg;
		Message::Args args;
		args.add("EXCEPTION ERROR on sendHeartbeat: Caught unknown exception!");
		msg.format(args);
		ml.logErrorMessage(msg);
	}
*/
	// loop forever reading the hardware status
	while(TRUE)
	{
		// parse output file
	
		ifstream File ("/tmp/harwareMonitor.txt");
		if (!File){
			// System error, Log this event 
			LoggingID lid(SERVER_MONITOR_LOG_ID);
			MessageLog ml(lid);
			Message msg;
			Message::Args args;
			args.add("Error opening /tmp/harwareMonitor.txt!!!");
			msg.format(args);
			ml.logWarningMessage(msg);
			sleep(300);
			continue;
		}
		
		char line[200];
		while (File.getline(line, 200))
		{
			// parse the line
			int f = 0;
			p = strtok(line,"|");
			while (p) 
			{
				data[f]=p;
				data[f] = serverMonitor.StripWhitespace(data[f]);
				p = strtok (NULL, "|");
				f++;
			}
	
			if( f == 0 )
				// nothing on this line, skip
				continue;
	
			SensorName = data[0];
			SensorValue = atof(data[1].c_str());
			Units = data[2];
			SensorStatus = data[3];
			lowFatal = atof(data[4].c_str());
			lowCritical = atof(data[5].c_str());
			lowWarning = atof(data[6].c_str());
			highWarning = atof(data[7].c_str());
			highCritical = atof(data[8].c_str());
			highFatal = atof(data[9].c_str());

			// check status and issue apporiate alarm if needed
			if ( (SensorStatus != "ok") && (SensorStatus != "nr") && (SensorStatus != "na") ) {
				// Status error, check for warning or critical levels

				if ( SensorValue >= highFatal ) {
					// issue critical alarm and send message to shutdown Server
					serverMonitor.sendAlarm(SensorName, HARDWARE_HIGH, SET, SensorValue);
					serverMonitor.sendMsgShutdownServer();
				}
				else if ( (SensorValue < highFatal) && (SensorValue >= highCritical) )
					// issue major alarm
					serverMonitor.sendAlarm(SensorName, HARDWARE_MED, SET, SensorValue);

				else if ( (SensorValue < highCritical ) && (SensorValue >= highWarning) )
					// issue minor alarm
					serverMonitor.sendAlarm(SensorName, HARDWARE_LOW, SET, SensorValue);

				else if ( (SensorValue <= lowWarning) && (SensorValue > lowCritical) )
					// issue minor alarm
					serverMonitor.sendAlarm(SensorName, HARDWARE_LOW, SET, SensorValue);

				else if ( (SensorValue <= lowCritical) && (SensorValue > lowFatal) )
					// issue major alarm
					serverMonitor.sendAlarm(SensorName, HARDWARE_MED, SET, SensorValue);

				else if ( SensorValue <= lowFatal ) {
					// issue critical alarm and send message to shutdown Server
					serverMonitor.sendAlarm(SensorName, HARDWARE_HIGH, SET, SensorValue);
					serverMonitor.sendMsgShutdownServer();
				}
				else
					// check if there are any active alarms that needs to be cleared
					serverMonitor.checkAlarm(SensorName);
			}
			else
				// check if there are any active alarms that needs to be cleared
				serverMonitor.checkAlarm(SensorName);

		} //end of parsing file while
		
		File.close();

		// send heartbeat message
/*		try {
			ProcHeartbeat procheartbeat;
			procheartbeat.sendHeartbeat(HW_HEARTBEAT_ID);

			LoggingID lid(SERVER_MONITOR_LOG_ID);
			MessageLog ml(lid);
			Message msg;
			Message::Args args;
			args.add("Sent Heartbeat Msg");
			msg.format(args);
			ml.logDebugMessage(msg);
		}
		catch (exception& ex)
		{
			string error = ex.what();
			if ( error.find("Disabled") == string::npos ) {
				LoggingID lid(SERVER_MONITOR_LOG_ID);
				MessageLog ml(lid);
				Message msg;
				Message::Args args;
				args.add("EXCEPTION ERROR on sendHeartbeat: ");
				args.add(error);
				msg.format(args);
				ml.logErrorMessage(msg);
			}
		}
		catch(...)
		{
			LoggingID lid(SERVER_MONITOR_LOG_ID);
			MessageLog ml(lid);
			Message msg;
			Message::Args args;
			args.add("EXCEPTION ERROR on sendHeartbeat: Caught unknown exception!");
			msg.format(args);
			ml.logErrorMessage(msg);
		}
*/
		// sleep
		sleep(MONITOR_PERIOD);
	} //end of forever while loop
}
