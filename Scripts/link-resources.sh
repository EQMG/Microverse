cd ../

SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
mkdir ${SCRIPTPATH}/Build/Debug/bin/Resources
ln -s ${SCRIPTPATH}/Libraries/Acid/Resources ${SCRIPTPATH}/Build/Debug/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Debug/bin/Resources/Game
ln -s ${SCRIPTPATH}/Libraries/Acid/Resources ${SCRIPTPATH}/Build/Release/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/Release/bin/Resources/Game
ln -s ${SCRIPTPATH}/Libraries/Acid/Resources ${SCRIPTPATH}/Build/RelWithDebInfo/bin/Resources
ln -s ${SCRIPTPATH}/Resources ${SCRIPTPATH}/Build/RelWithDebInfo/bin/Resources/Game
