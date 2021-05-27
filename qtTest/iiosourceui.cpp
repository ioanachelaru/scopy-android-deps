#include "iiosourceui.h"
#include "ui_iiosourceui.h"

#include <boost/shared_ptr.hpp>
#include <gnuradio/hier_block2.h>

#include <gnuradio/io_signature.h>
#include <gnuradio/iio/device_source.h>
#include <gnuradio/blocks/short_to_float.h>

class iio_float_src : public gr::hier_block2
{

	gr::iio::device_source::sptr iio_blk;
	gr::blocks::short_to_float::sptr s2f;
public:
	typedef boost::shared_ptr<iio_float_src> sptr;

	iio_float_src(std::string ctx, std::string dev, std::vector<std::string> channels, std::string phy) : hier_block2("iio float src",
				     gr::io_signature::make(0, 0, sizeof(float)),
				     gr::io_signature::make(1, 2, sizeof(float))) {
		std::vector<std::string> params;
		iio_blk = gr::iio::device_source::make(ctx,dev,channels,phy,params);
		s2f = gr::blocks::short_to_float::make();
		connect(iio_blk,0,s2f,0);
		connect(s2f,0,self(),0);
	}
	~iio_float_src() {}
	static sptr make(std::string ctx, std::string dev, std::vector<std::string> channels, std::string phy) {
		return gnuradio::get_initial_sptr(new iio_float_src(ctx,dev,channels,phy));
	}

};

IioSourceUi::IioSourceUi(adiscope::DataSource *ds, QWidget *parent) :
	QWidget(parent),
	ds(ds),
	ui(new Ui::IioSourceUi)
{
	ui->setupUi(this);
}

IioSourceUi::~IioSourceUi()
{
	delete ui;
}

void IioSourceUi::on_pushButton_clicked()
{

	auto iio_blk = iio_float_src::make("ip:192.168.2.1","cf-ad9361-lpc",{"voltage0"},"ad9361-phy");
	ds->man->getTopBlock("test")->replaceBlock(ds->block, iio_blk);
	ds->block = iio_blk;

}


