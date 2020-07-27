#ifndef _ISIGNAL_H
#define _ISIGNAL_H



#include <vector>

/*
	 this class for signals between obj of baseClass
*/

template<class T, class R ,typename U> // T class of obj that send signals R class of object thats hand them and U is a type of signal
class ISignal
{
public:
	typedef U*(T::* SIGNAL) (U*); // signal type
	typedef void (R::* HANDLER) (U*); // handler type


	struct o_sh // struct for store information about signal and handlers
	{
		SIGNAL signal;
		R* p_second_obj;
		HANDLER handler;
		o_sh() {};
		o_sh(SIGNAL signal, R* p_second_obj, HANDLER handler)
		{
			this->signal = signal;
			this->p_second_obj = p_second_obj;
			this->handler = handler;
		};
	};

	void set_connect(SIGNAL signal, R* p_second_obj, HANDLER handler) // set connect between signal and handler
	{
		o_sh* tmp = new o_sh(signal, p_second_obj, handler);

		if (!connects.empty())
		{
			for (o_sh* obj : connects)
			{
				if (obj == tmp) return;
			}
		}
		connects.push_back(tmp);
	}

	friend void raise(T* obj, SIGNAL signal, R* sObj, HANDLER handler, U* sig)
	{
		(sObj->*handler)((obj->*signal)(sig));
	}

	void emit_signal(SIGNAL signal, U* sig)
	{
		for (o_sh* obj : this->connects)
		{
			if (obj->signal == signal)
			{
				raise((T*)this, signal, obj->p_second_obj, obj->handler, sig);
			}
		}
	}

	void delete_connect(SIGNAL signal, R* sObj, HANDLER handler)
	{
		if (connects.empty()) return;

		o_sh* tmp = new o_sh(signal, sObj, handler);
		unsigned int i = 0;
		bool isFind = false;
		for (i = 0; i != connects.size(); i++)
		{
			if (connects[i] == tmp)
			{
				isFind = true;
				break;
			}
		}

		if (isFind) connects.erase(connects.begin() + i);
	}

protected:
	std::vector<o_sh*> connects; // container for store structs
};

#endif // !_ISIGNAL_H
