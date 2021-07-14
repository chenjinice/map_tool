// This file is part of OpenC2X.
//
// OpenC2X is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenC2X is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with OpenC2X.  If not, see <http://www.gnu.org/licenses/>.
//
// Authors:
// Sven Laux <slaux@mail.uni-paderborn.de>
// Gurjashan Singh Pannu <gurjashan.pannu@ccs-labs.org>
// Stefan Schneider <stefan.schneider@ccs-labs.org>
// Jan Tiemann <janhentie@web.de>


#include "CommunicationReceiver.h"

using namespace std;

CommunicationReceiver::CommunicationReceiver(string portIn,string ip,string envelope) {

	mEnvelope = envelope;
	mContext = new zmq::context_t(1);
	mSubscriber = new zmq::socket_t(*mContext, ZMQ_SUB);

	mSubscriber->connect(("tcp://" + ip + ":" + portIn).c_str());
    	cout << ("tcp://" + ip + ":" + portIn).c_str() << "filter :" << envelope.c_str() << endl;
    
	if (envelope == "") {
		mSubscriber->setsockopt(ZMQ_SUBSCRIBE, "", 0);//subscribe to all messages
	} else {
		mSubscriber->setsockopt(ZMQ_SUBSCRIBE, envelope.c_str(), 1);
	}
    //int tcp_keep_alive = 1;
    //mSubscriber->setsockopt(ZMQ_TCP_KEEPALIVE, &tcp_keep_alive, sizeof(tcp_keep_alive));
    //int tcp_keep_idle = 120;
    //mSubscriber->setsockopt(ZMQ_TCP_KEEPALIVE_IDLE, &tcp_keep_idle, sizeof(tcp_keep_idle));
}

CommunicationReceiver::~CommunicationReceiver() {
	mContext->close();
	mSubscriber->close();
	delete mContext;
	delete mSubscriber;
}

pair<string, string> CommunicationReceiver::receive_poll() {
	//Poll socket for a reply, with timeout
	zmq::pollitem_t items[] = { { (void*) *mSubscriber, 0, ZMQ_POLLIN, 0 } };
	zmq::poll (&items[0], 1, 2000);
	//If we got a reply, process it
	if (items[0].revents & ZMQ_POLLIN) {
		//  We got a reply from the server, return it to requester
		string envelope = s_recv(*mSubscriber);
		string message = s_recv(*mSubscriber);

		return make_pair(envelope, message);
	}
    //could not get a reply, return ""
	return make_pair("", "");;

}

pair<string, string> CommunicationReceiver::receive() {
	string envelope = s_recv(*mSubscriber);
	string message = s_recv(*mSubscriber);

	return make_pair(envelope, message);
}

string CommunicationReceiver::receiveFromHw() {
	string message = s_recv(*mSubscriber);

	return message;
}

string CommunicationReceiver::receiveData() {
	string envelope = s_recv(*mSubscriber);
	string message = s_recv(*mSubscriber);
	return message;
}

void CommunicationReceiver::close_conn() {
	mSubscriber->close();
	delete mSubscriber;
	mContext->close();	
	delete mContext;	
}

