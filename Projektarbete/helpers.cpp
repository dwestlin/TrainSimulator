#include "helpers.h"



//G�r om str�ngen till integer.
int helpers::fromStringtoInt(string &str)
{
	int hr, min;
	char c;
	istringstream iss(str);

	iss >> hr >> c >> min;

	int time = hr * 60 + min;

	return time;
}


string helpers::fromInttoString(int time)
{

	// Om tiden som ska g�ras om �r st�rre eller lika med 1440 (som �r kl 24:00), d� ska den stoptiden dra av den faktiska tiden f�r att den ska b�rja p� nytt dygn 
	if (time >= STOP_TIME)
	{
		int tempTime = time - STOP_TIME;

		int hr = tempTime / 60;
		int min = (tempTime % 60);

		string h, m;

		if (hr == 0)
			h = "00";
		else if (hr < 10 && hr > 0)
			h = "0" + to_string(hr);
		else
			h = to_string(hr);

		if (min == 0)
			m = "00";
		else if (min < 10 && min > 0)
			m = "0" + to_string(min);
		else
			m = to_string(min);

		return h + ":" + m;
	}
	else
	{
		int hr = time / 60;
		int min = (time % 60);

		string h, m;

		if (hr == 0)
				h = "00";
		else if (hr < 10 && hr > 0)
			h = "0" + to_string(hr);
		else
			h = to_string(hr);

		if (min == 0)
			m = "00";
		else if (min < 10 && min > 0)
			m = "0" + to_string(min);
		else
			m = to_string(min);

		return h + ":" + m;
	}
}
