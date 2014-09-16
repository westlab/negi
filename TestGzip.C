/* sample test program
int main(){

	Gzip Gzip;
	FILE *fin, *fout;
	char finbuf[FILEBUFSIZ];
	char foutbuf[FILEBUFSIZ];
	z_stream *z;
	z = (z_stream *)malloc(sizeof(z_stream));

	if(( fin = fopen("hoge.gz","r")) == NULL){
		fprintf(stderr, "Can't open read file\n");
		exit(1);
	}
	if(( fout = fopen("hoge.plain","w")) == NULL){
		fprintf(stderr, "Can't open write file\n");
		exit(1);
	}
	
	int size;
	int outsize;

	int offset = -1;


	while(size = fread(finbuf, 1, TMPBUFSIZ, fin)){
		cout << "hoge" <<size << endl;
		if(offset == -1){
			Gzip.dec_init(z);
			offset = Gzip.dec_gzip(foutbuf,finbuf,size,z);
		}
		outsize = Gzip.dec_deflate(foutbuf,finbuf + offset, size - offset,z);
		offset = 0;
		cout << size << endl;
		cout << outsize << endl;
		fwrite(foutbuf,1,outsize,fout);
	}
	
}

*/
