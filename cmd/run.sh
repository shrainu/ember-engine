cd ../product
rm $1
cd ../cmd
./build.sh
cd ../product
./$@