using namespace lslib::os;

cout << "get_module_path: " << get_module_path().c_str() << endl;
cout << "get_module_name: " << get_module_name().c_str() << endl;

cout << "get_slash: " << get_slash() << endl;
for (int i = 0; i < 128; i++)
{
	cout << "[" <<  i << "]" << (_lchar)i << " is_slash: " << is_slash(i) << endl;
}

lstring strsz[] = {
	"E:\\Mine\\Code\\lslib\\bin\\Debug",
	"E:\\Mine\\Code\\lslib\\bin\\Debug\\",
	"E:\\\\Mine\\\\Code\\\\lslib\\\\bin\\\\Debug\\\\",
	"E:/Mine/Code/lslib/bin/Debug",
	"E:/Mine/Code/lslib/bin/Debug/",
	"E:////Mine////Code////lslib////bin////Debug////",
	"E:////Mine////Code\\\\lslib////bin\\\\Debug////\\\\",
	"E:////Mine////Code\\\\lslib////bin\\\\test////\\\\",
	".",
	"./",
	".\\",
	".////",
	".\\\\",
	".////\\\\",
	"..",
	"../Debug",
	"test.exe",
	"../Debug/\\test.exe",
	"E:\\Mine\\Code\\lslib\\bin\\Debug\\test.exe",
	"E:\\Mine\\Code\\lslib\\bin\\Debug\\test//test.exe",
};
for (int i = 0; i < sizeof(strsz)/sizeof(lstring); i++)
{
	const lstring& str = strsz[i];
	cout << "path_get_dir(\"" << str << "\"): " << path_get_dir(str).c_str() << endl;
	cout << "path_get_name(\"" << str << "\"): " << path_get_name(str).c_str() << endl;
	cout << "path_get_filename(\"" << str << "\"): " << path_get_filename(str).c_str() << endl;
	cout << "path_get_ext(\"" << str << "\"): " << path_get_ext(str).c_str() << endl;
	cout << "path_pretty(\"" << str << "\"): " << path_pretty(str).c_str() << endl;
	cout << "path_make_absolute(\"" << str << "\"): " << path_make_absolute(str).c_str() << endl;
	cout << "is_exist(\"" << str << "\"): " << is_exist(str) << endl;
	cout << "is_file(\"" << str << "\"): " << is_file(str) << endl;
	cout << "is_dir(\"" << str << "\"): " << is_dir(str) << endl;
	cout << "------------------" << endl;
}

