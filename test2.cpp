#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <tuple>
#include <map>

using namespace std;

tuple<vector<char*>,vector<char*>,vector<char*>> SearchFileFileds(string filePath)
{
	vector<char*> resurls;
	vector<char*> resip;
	vector<char*> resf;

	fstream fs;
	fs.open(filePath);
	if (fs.fail())
	{
		fs.close();
		cout << "fstream fail!" << endl;
		return make_tuple(resurls,resip,resf);
	}

	string fileString;

	while (getline(fs, fileString))  //读取一行
	{
		char* p = new char[fileString.length()];

		memset(p, '\0', sizeof(p));
		strcpy(p, fileString.c_str());

		//提取url
		char* ur = new char[fileString.length()];
		ur = p;
		char j, k, h, l;
		
		string st = "http";
		char* sst = new char[5];

		while (*ur != '\0') {
			if (4 == sscanf(ur, "%c%c%c%c", &j,&k,&h,&l)) {
				sst[0] = j;
				sst[1] = k;
				sst[2] = h;
				sst[3] = l;
				sst[4] = '\0';
				if (st._Equal(sst)) {
					char* out_ip = new char[2000];
					char* out = out_ip;

					int num = 0;
					bool flag = false;
					while ((*ur) != '\"' && (*ur) != ' ' && (*ur) != '\0' && (*ur) != ',') {
						if ((num == 4) && (*ur == 's' || *ur == ':') ){
							flag = true;;
						}
						num++;
						*out_ip = *ur;
						out_ip++;
						*out_ip = '\0';
						ur++;
					}
					if(flag)
						resurls.push_back(out); 
				}
			}
			if (*ur != '\0') {
				ur++;
			}
		}

		//提取ipV4地址
		char* ps = new char[fileString.length()];
	    ps = p;
		int a, b, c, d;
	    bool set = false;

		for (int i = 0; (*ps)!='\0'; i++) {
			if ((*ps >= '0') && (*ps <= '9') && !set) {
				if (4 == sscanf(ps, "%d.%d.%d.%d", &a, &b, &c, &d)) {
					if (0 <= a && a <= 255
						&& 0 <= b && b <= 255
						&& 0 <= c && c <= 255
						&& 0 <= d && d <= 255) {

						char* out_ip = new char[40];
						memset(out_ip, '\0', sizeof(out_ip));

						sprintf(out_ip, "%d.%d.%d.%d", a, b, c, d);
						resip.push_back(out_ip);
						set = true;
					}
				}
			}
			else{
				set = false;
			}
			ps++;
		}

		//提取进度信息
		char* pos = new char[fileString.length()];
		pos = p;
		int m, n;

		while (*(pos++) != '\0') {
			if (2 == sscanf(pos, "%d/%d", &m, &n)) {
				resf.push_back(p);
				break;
			}
		}
	}
	
	fs.close();

	return make_tuple(resurls,resip,resf);
}

int main()
{
	string filePath = "D:\\aboutjob\\测试题\\metaapp\\log.txt";

	vector<char*> resurls;
	vector<char*> resip;
	vector<char*> resf;

	auto tup = SearchFileFileds(filePath);
	resurls = get<0>(tup);
	resip = get<1>(tup);
	resf = get<2>(tup);

	ofstream os;     //创建一个文件输出流对象
	os.open("20240123.txt");//将对象与文件关联

	//处理url
	if (resurls.empty()) {
		cout << "No urls" << endl;
	}
	else {
		cout <<"可重复uri个数："<<resurls.size() << endl;

		set<string> map;
		os << "去重前uri个数：" << resurls.size() << endl;
		for (const auto& it:resurls) {
			cout << it << endl;
			os << it << endl;
			string s = it;
			map.insert(s);
		}
		os << "去重前uri个数：" << map.size() << endl;
		cout << "无重复url个数："<<map.size() << endl;
	}

	//处理ip地址
	if (resip.size() == 0)
	{
		cout << "Compare wrong!" << endl;
	}
	else
	{
		cout << "可重复ip地址个数"<<resip.size() << endl;
		set<string> map;
		for (const auto& it:resip) {
			cout << it << endl;
			string s = it;
			map.insert(s);
		}
		cout<< "无重复ip地址个数"<<map.size()<<endl;
		os << "去重后ip地址个数:" << map.size() << endl;
		for (const auto& it:map) {
			cout << it << endl;
			os << it<<endl;
		}
	}

	//处理进度
	if (resf.empty()) {
		cout << "No F" << endl;
	}
	else {
		os << "进度对应log列：" << endl;
		for (const auto& it : resf) {
			cout << it << endl;
			os << it<<endl;
		}
	}

	os.close();

	return 0;
}


