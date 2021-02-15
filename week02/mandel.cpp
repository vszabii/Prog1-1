#include <complex>
#include <iostream>
#include <png++/image.hpp>
#include <png++/png.hpp>
#include <png++/rgb_pixel.hpp>

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage: ./mandelpng output_filename\n";
        return 1;
    }

    double domain_lower = -2.0, domain_upper = 0.5, codomain_lower = -1.35,
           codomain_upper = 1.35;

    unsigned int picture_width = 600, picture_height = 600,
                 iteration_limit = 1000;

    png::image<png::rgb_pixel> image{picture_width, picture_height};

    double scale_x = (domain_upper - domain_lower) / picture_width;
    double scale_y = (codomain_upper - codomain_lower) / picture_height;

    unsigned int completed_iterations = 0;

    std::cout << "Calculating";

    std::complex<double> C, Z, newZ;

    for (int j = 0; j < image.get_height(); ++j)
        for (int k = 0; k < image.get_width(); ++k) {
            C = {domain_lower + k * scale_x, codomain_upper - j * scale_y};

            Z = {0, 0};
            completed_iterations = 0;

            while (abs(Z) < 2 && completed_iterations < iteration_limit) {
                Z = Z * Z + C;
                ++completed_iterations;
            }

            image.set_pixel(k, j,
                            png::rgb_pixel(255 - completed_iterations % 256,
                                           255 - completed_iterations % 256,
                                           255 - completed_iterations % 256));
        }

    image.write(argv[1]);
    std::cout << argv[1] << " mentve" << std::endl;
}