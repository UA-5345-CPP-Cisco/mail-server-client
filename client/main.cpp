#include <QApplication>
#include "screens/registrationscreen.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	ISXUi::RegistrationScreen screen;
	screen.resize(400, 350);
	screen.setWindowTitle("Creating account");
	screen.show();

	return app.exec();
}