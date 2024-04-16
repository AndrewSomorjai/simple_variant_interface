/*
 * irrlicht_variant_1.cpp
 *
 *  Created on: Apr 15, 2024
 *      Author: verthex
 */

#include <iostream>
#include <string>
#include <type_traits>
#include <boost/variant.hpp>
#include <irrlicht.h>

using namespace irr;

video::IVideoDriver* driver;
typedef boost::variant< int, double, std::string, video::IVideoDriver*> TVariant;

template <typename application>
class interface
{
	private:

		application data_type;

	public:

	interface(){}
	interface( const application & data_type)
	{
		this->data_type = data_type;
	}
	~interface(){}
	application get_data_type() const
	{
		return data_type;
	}
};
template<class Result, class Func>
struct forwarding_visitor : boost::static_visitor<Result>
{
    Func func;
    forwarding_visitor( const Func& f):func(f)
    {
    	std::cout<<"const Func& f"<<std::endl;
    }
    forwarding_visitor( Func&& f):func( std::move(f))
    {
    	std::cout<<"Func&& f "<<std::endl;
    }
    template<class Arg>
    Result operator()(Arg && arg) const
    {
        return func( std::forward<Arg>(arg));
    }
};
template<class Result, class Func>
forwarding_visitor< Result, std::decay_t<Func> > make_forwarding_visitor( Func && func)
{
    return { std::forward<Func>(func)};
}
int main()
{
	std::vector< boost::variant<
	interface<double>,
	interface<std::string>,
	interface<int>,
	interface<video::IVideoDriver* > > > vec;

	interface<double> DOUBLE(1.423423);
	vec.push_back( DOUBLE);

	auto visitor = make_forwarding_visitor< TVariant>([](const auto& t){ return t.get_data_type();});

	for( unsigned int index = 0; index < vec.size(); index++)
		std::cout<<boost::apply_visitor ( visitor, vec[index] )<<std::endl;
}







