
float str2float(const unsigned char *str){
	unsigned char state=0;
	float data=0.0f;
	float dec=1.0f;
	float neg=1.0f;
	while(*str){
		switch (state){
			case 0:
				if ((*str)=='-'){
					neg=1;
				}else if ((*str)>='0' && (*str)<='9'){
					data *=10.0f;
					data += (*str)-0x30;
				}else if ((*str)=='.'){
					state=1;
				}else{
					data *=neg;
					return data;
				}
				break;
			case 1:
				if ((*str)>='0' && (*str)<='9'){
					dec*=0.1f;
					data += ((*str)-0x30)*dec;
				}else{
					data *=neg;
					return data;
				}
				break;
			}
		str++;
	}
	data *=neg;
	return data;
}


void gll2int(const unsigned char *str, int *lat, int *lon){
	unsigned char state=0;
	float data=0.0f;
	float dec=1.0f;
	//float neg=1.0f;
	while(*str){
		switch (state){
			case 0:
				if ((*str)>='0' && (*str)<='9'){
					data *=10.0f;
					data += (*str)-0x30;
				}else if ((*str)=='.'){
					state=1;
				}
				break;
			case 1:
				if ((*str)>='0' && (*str)<='9'){
					dec*=0.1f;
					data += ((*str)-0x30)*dec;
				}else if ((*str)=='N'){
					
					*lat = (int)(data*100000);
					data=0.0f;
					dec=1.0f;
					state=2;
				}else if ((*str)=='S'){
					*lat = (int)(data*(-100000));
					data=0.0f;
					dec=1.0f;
					state=2;
				}else{
					return;
				}
				break;
			case 2:
				if ((*str)>='0' && (*str)<='9'){
					data *=10.0f;
					data += (*str)-0x30;
				}else if ((*str)=='.'){
					state=3;
				}
				break;
			case 3:
				if ((*str)>='0' && (*str)<='9'){
					dec*=0.1f;
					data += ((*str)-0x30)*dec;
				}else if ((*str)=='W'){
					*lon = (int)(data*(-100000));
					return;
				}else if ((*str)=='E'){
					*lon = (int)(data*100000);
					return;
				}
				break;
			}
		str++;
	}
}
