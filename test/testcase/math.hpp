using namespace lslib::math;


double sz[] = {
	0.1234567890123456789,
	12.345678901234567890,
	234.56789012345678901,
	3456.7890123456789012,
	45678.901234567890123,
	567890.12345678901234,
	6789012.3456789012345,
	78901234.567890123456,
	890123456.78901234567,
	9012345678.9012345678,
};

for (int i = 0; i < sizeof(sz)/sizeof(double); i++)
{
	cout <<"Round(" << setiosflags(ios::fixed) << setprecision(11) <<sz[i] << ", " << i << "): " << Round(sz[i], i) << endl;
	cout <<"Round(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Round(-sz[i], i) << endl;
	cout <<"Floor(" << setiosflags(ios::fixed) << setprecision(11) << sz[i] << ", " << i << "): " << Floor(sz[i], i) << endl;
	cout <<"Floor(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Floor(-sz[i], i) << endl;
	cout <<"Ceil(" << setiosflags(ios::fixed) << setprecision(11) << sz[i] << ", " << i << "): " << Ceil(sz[i], i) << endl;
	cout <<"Ceil(" << setiosflags(ios::fixed) << setprecision(11) << -sz[i] << ", " << i << "): " << Ceil(-sz[i], i) << endl;
	cout << "------------------" << endl;
}
