cd ../

mkdir Build\Debug\bin\Resources

mklink /J Build\Debug\bin\Resources\Engine Libraries\Flounder\Resources
mklink /J Build\Debug\bin\Resources\Game Resources
mklink /J Build\Release\bin\Resources\Engine Libraries\Flounder\Resources
mklink /J Build\Release\bin\Resources\Game Resources
mklink /J Build\RelWithDebInfo\bin\Resources\Engine Libraries\Flounder\Resources
mklink /J Build\RelWithDebInfo\bin\Resources\Game Resources

mklink /J Build\Sources\Microverse\Resources\Engine Libraries\Flounder\Resources
mklink /J Build\Sources\Microverse\Resources\Game Resources
