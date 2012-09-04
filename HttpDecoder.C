/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: HttpDecoder.C,v 5.8 2011-11-18 04:36:20 sin Exp $
*/

#include "Include.H"
#include "Packet.H"
#include "Stream.H"
#include "Gzip.H"
#include "HttpDecoder.H"
#include "Global.H"


HttpDecoder::HttpDecoder(){
	localbuf = (u_char *)malloc(sizeof(SAVE_MAX_SIZE));
	return;
}

HttpDecoder::~HttpDecoder(){
	free(localbuf);
	return;
}


void HttpDecoder::Proc(Packet *packet){
	if(packet->GetStream() == NULL){ return; }

	Stream *stream = packet->GetStream();

	HTTP_DEBUG(MSG("Analyze");)
	if(stream->GetState() == BEGIN) AnalyzeHeader(packet);
	HTTP_DEBUG(MSG("Chunk");)
	if(stream->GetHttpChunked()) DecodeChunk(packet);
	HTTP_DEBUG(MSG("Gzip"));
	if(!stream->GetL7Error()){
		if(stream->GetHttpCompress()) DecodeGzip(packet);
	}
}

void HttpDecoder::AnalyzeHeader(Packet *packet){
	Stream *stream = packet->GetStream();
	char* content = (char *)packet->GetContent();

	//Is this stream for port 80?
	if(!(stream->GetDstPort() == 80 || stream->GetDstPort() == 8080 || stream->GetSrcPort() == 80 || stream->GetSrcPort() == 8080)) return;

	//Search for 'HTTP/1.1'
	if(strncmp(content, "HTTP/1.1", sizeof("HTTP/1.1")-1) != 0){
		//if not match, return
		return;
	}else{
		stream->SetL7Protocol(HTTP);
	}

	//Search for header end(CR LF CR LF)
	char *header_end = strstr(content, "\r\n\r\n");
	if(header_end ==NULL){
		return;
	}else{
		stream->SetHttpHeaderSize((header_end - content) + sizeof("\r\n\r\n")-1);
		if(packet->GetContentSize() == stream->GetHttpHeaderSize()){
			//there is HTTP header, but no content.
			return;
		}
	}

	//Search for Transfer-Encoding: chunked
	if(strstr(content, "Transfer-Encoding: chunked") != NULL){
		stream->SetHttpChunked(1);
	}

	//Search for Content-Length
	char *res = strstr(content, "Content-Length: ");
	if(res != NULL){
		res += sizeof("Content-Length: ")-1;
		stream->SetHttpContentSize(atol(res));
	}

	//Search for Content-Encoding: gzip|deflate
	if(strstr(content, "Content-Encoding: gzip") != NULL){
		stream->SetHttpCompress(GZIP);
	}else if(strstr(content, "Content-Encoding: DEFLATE") != NULL){
		stream->SetHttpCompress(DEFLATE);
	}
}

void HttpDecoder::DecodeChunk(Packet *packet){
	HTTP_DEBUG(MSG("Chunk Start"));
	Stream *stream = packet->GetStream();
	u_char* content = packet->GetContent();

	u_char *l7_content = (u_char*)malloc(packet->GetContentSize());	//enough size for dechunk v4 packet.
	packet->SetL7Content(l7_content);

	u_char *p_l7_content_work;
	u_char *p_content_work;
	//u_int l7_content_size = packet->GetContentSize();
	u_int l7_content_size = 0;

	memcpy(l7_content, content, packet->GetContentSize());

	if(stream->GetState() == BEGIN){
		p_content_work = content + stream->GetHttpHeaderSize();
		p_l7_content_work = l7_content + stream->GetHttpHeaderSize();
		l7_content_size = stream->GetHttpHeaderSize();
//		MSG(content);
	}else if(stream->GetState() == END){
		return;
	}else{
		//large chank
		if(packet->GetContentSize() < stream->GetChunkOffset()){
			stream->SetChunkOffset(stream->GetChunkOffset() - packet->GetContentSize());
			packet->SetL7ContentSize(packet->GetContentSize());
			return;
		}
		p_content_work = content + stream->GetChunkOffset(); //GetChunkOffset is rest size of devided chank
		p_l7_content_work = l7_content + stream->GetChunkOffset();
		l7_content_size = stream->GetChunkOffset();
	}
	
	u_int chunk_size = 0;

	//while working pointer - start pointer > packet's http content's size
	//while(p_content_work - content < packet->GetContentSize() ){
	while(1){
	HTTP_DEBUG(MSG("Chunk Loop"));
		//get chunk size.
		u_char * first_nonnum;


//		u_char *temp = p_content_work;
//			cout << "before decode: " << p_content_work << endl;
		chunk_size = strtol((const char *)p_content_work, (char **)&first_nonnum, 16); //10 is  base of hexadecimal(0x) number.

		p_content_work = first_nonnum + sizeof("\r\n")-1;
		if(p_content_work - content  >= packet->GetContentSize()){
			stream->SetChunkOffset(chunk_size);
			return;
		}
		if(strstr((char *)first_nonnum,"\r\n") != (char*)first_nonnum){
			p_content_work = (u_char*)strstr((char *)first_nonnum,"\r\n") + sizeof("\r\n")-1;
			if(p_content_work - first_nonnum > 10 || p_content_work - first_nonnum < 0){
//				MSG("too far from chunk size!!");
				return;
			}

//			MSG("first_nonnum:"<<first_nonnum)
//			MSG("content:"<<content);
//			//MSG("contentwork:"<<(p_content_work-content));
//			MSG("contentwork:"<<(p_content_work-first_nonnum));
//			MSG("contentwork:"<<(int)p_content_work[0]);
//			MSG("headersize:"<<stream->GetHttpHeaderSize());
		}

		//debug
		if(chunk_size == LONG_MAX || chunk_size == LONG_MIN){
				//chunk size error!
//				cout << "error state: "<< stream->GetState() << endl;
//				cout << "error offset: "<< stream->GetChunkOffset() << endl;
				cout << "error chunk size: "<< chunk_size << endl;
//				cout << "error content size: "<< packet->GetContentSize() << endl;
//				cout << "error calc size: "<<first_nonnum - content << endl;
//				cout << "error calc size: "<<p_content_work - content << endl;
//				MSG(content);
				MSG(first_nonnum);
//				MSG(p_content_work);
				stream->SetL7Error(1);
				return;
				exit(1);
		}

		if(chunk_size == 8){
//				cout << "error content: "<<content << endl;
//				cout << "error p_content_work: "<<p_content_work << endl;
			}

		if(chunk_size == 0){
			//message is end.
			stream->SetChunkOffset(0);
			packet->SetL7ContentSize(l7_content_size);
			return;	

		}else if(p_content_work - content + chunk_size > packet->GetContentSize()){
			//chunk size > packet end. save offset and return.
			u_int save_size = packet->GetContentSize() - (p_content_work - content);
			u_int offset_size = chunk_size - save_size;
			memcpy(p_l7_content_work, p_content_work, save_size);
			stream->SetChunkOffset(offset_size);
			packet->SetL7ContentSize(l7_content_size + save_size);
			return;

		}else{
			//Copy chunk and goto next chunk
			memcpy(p_l7_content_work, p_content_work, chunk_size);
			p_l7_content_work += chunk_size;
			l7_content_size += chunk_size;
			p_content_work += chunk_size + sizeof("\r\n")-1;
			//stream->SetChunkOffset(0);
			//chunk_size = 0;
		}
	}
}

void HttpDecoder::DecodeGzip(Packet *packet){
	Stream *stream = packet->GetStream();

	u_char* p_dec_start = packet->GetContent();
	z_stream *z = stream->GetGzipZ();
	int insize = packet->GetContentSize();
	int outsize = 0;
	int offset = 0;

	if(packet->GetL7Content() != packet->GetContent()){
		p_dec_start = packet->GetL7Content();
		insize = packet->GetL7ContentSize();
	}
	if(stream->GetState() == BEGIN){
		insize -= stream->GetHttpHeaderSize();
		p_dec_start += stream->GetHttpHeaderSize();
		z = (z_stream*)malloc(sizeof(z_stream));
		stream->SetGzipZ(z);
		gzip.dec_init(z);

		if( stream->GetHttpCompress() == GZIP){
			offset = gzip.dec_gzip(localbuf, p_dec_start, insize, z);
		}else if( stream->GetHttpCompress() == DEFLATE){
			offset = gzip.dec_zlib(localbuf, p_dec_start, insize, z);
		}
		if(offset <= 0){
			stream->SetL7Error(1);
			return;
		}
	}
//	packet->Show();
//	cout << "content size: "<< packet->GetContentSize() << endl;
//	cout << "insize: "<< insize << endl;
//	cout << "httpheadersize: "<< stream->GetHttpHeaderSize() << endl;
//	cout << "offset: "<< offset << endl;

	//WARINIG!! localbuf is not used!!
	outsize = gzip.dec_deflate(localbuf, p_dec_start+offset, insize - offset, z);
	if(outsize <= 0){
		return;
	}
	
	u_char *content = packet->GetContent();

	u_char *old_l7content = NULL;
	if(packet->GetL7Content() != packet->GetContent()){	
		content = packet->GetL7Content();
		old_l7content = packet->GetL7Content();

	}

	u_char *l7content = NULL;
	if(stream->GetState() == BEGIN){
		l7content = (u_char *)malloc(outsize + stream->GetHttpHeaderSize());
		memcpy(l7content, content , stream->GetHttpHeaderSize());
		memcpy(l7content + stream->GetHttpHeaderSize(), gzip.outbuffer , outsize);
		packet->SetL7Content(l7content);
		packet->SetL7ContentSize(stream->GetHttpHeaderSize() + outsize);
	}else{
		l7content = (u_char *)malloc(outsize + stream->GetHttpHeaderSize());
		memcpy(l7content, gzip.outbuffer, outsize);
		packet->SetL7Content(l7content);
		packet->SetL7ContentSize(outsize);
	}
//	MSG(packet->GetL7Content());
//	MSG(l7content);
//	MSG(localbuf);
	if(old_l7content != NULL){	
		free(old_l7content);
	}

	

	return;


}

