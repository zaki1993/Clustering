#include <iostream>
#include "lodepng/lodepng.h"
#include "lodepng/lodepng.cpp"

using namespace std;

std::vector<unsigned char> img;
unsigned w, h;	

std::vector<unsigned char> decodeOneStep(const char* filename)
{
    std::vector<unsigned char> image;
    unsigned width, height;

    //decode
    unsigned error = lodepng::decode(image, width, height, filename);
    cout << "w: " << width << " " << "h: " << height << endl;

    //if there's an error, display it
    if (error) std::cout << "decoder error " << error << ": " <<       lodepng_error_text(error) << std::endl;
    else
    {
        img = image;
        w = width;
        h = height;
        cout << "Success: " << filename << endl;
    }
    return image;
}

int main(){
	std::vector<unsigned char> v;
	v = decodeOneStep("parrot.png");
	for(auto i:v){
		//cout<<i<<endl;
	}
	const char* name = "asdas.png";
	lodepng::encode(name,v,471,600);
	return 0;
}