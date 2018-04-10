cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
ln -s ${SCRIPTPATH}/Libraries/Flounder/Resources ${SCRIPTPATH}/Build/Debug/bin/Resources
ln -s ${SCRIPTPATH}/Libraries/Flounder/Resources ${SCRIPTPATH}/Build/Release/bin/Resources
ln -s ${SCRIPTPATH}/Libraries/Flounder/Resources ${SCRIPTPATH}/Build/RelWithDebInfo/bin/Resources
