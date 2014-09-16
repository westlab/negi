/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: SSS.C,v 5.5 2012-08-22 08:05:52 sin Exp $
*/

#include "Include.H"
#include "Global.H"
#include "SSS.H"

void thread_sss(void *)
{
  Packet* pkt;
  int dstport;

  while(1) {
    // Waiting for packet to be arrived
    pthread_mutex_lock(&sss_cond_mut);
    pthread_cond_wait(&sss_cond, &sss_cond_mut);
    pkt = sss_send_value;
    pkt->Show();
    dstport = pkt->GetDstPort();
    cout << dstport << endl;
    /*
      cout << "Thread SSS Get Answer! "<< endl;
      //src ip
      cout << inet_ntoa(pkt->GetSrcIP()) << endl;
      //dst ip
      cout << inet_ntoa(pkt->GetDstIP()) << endl;
      }
    */
    pthread_mutex_unlock(&sss_cond_mut);
  }
  //	config->show();
}
