#include <iostream>
#include <grid.H>

Grid upwind(const int nx, const double u, const double C_in, const int num_periods) {

    double C = C_in;

    Grid g(nx, 1, 0.0, 1.0);

    // time information

    double dt = C * g.dx / u;
    double t{0.0};

    double tmax = num_periods * (g.xmax - g.xmin) / u;

    // initial the data to a tophat

    for (int i = g.ilo; i <= g.ihi; ++i) {
        if (g.x[i] > 1.0/3.0 && g.x[i] <= 2.0/3.0) {
            g.a[i] = 1.0;
        } else {
            g.a[i] = 0.0;
        }
    }


    // evolution loop

    std::vector<double> a_new(g.nq, 0.0);

    while (t < tmax) {

        if (t + dt > tmax) {
            dt = tmax - t;
            C = u * dt / g.dx;
        }

        // fill boundary conditions

        g.fill_BCs();

        // do upwinding update

        for (int i = g.ilo; i <= g.ihi; ++i) {
            a_new[i] = g.a[i] - C * (g.a[i] - g.a[i-1]);
        }

        // store the new solution on the grid

        for (int i = g.ilo; i <= g.ihi; ++i) {
            g.a[i] = a_new[i];
        }

        t += dt;

    }

    return g;
}


int main() {

    int nx{64};
    double u{1.0};
    double C{0.9};

    int num_periods{1};

    auto g = upwind(nx, u, C, num_periods);

    // output

    for (int i = g.ilo; i <= g.ihi; ++i) {
        std::cout << g.x[i] << "   " << g.a[i] << std::endl;
    }

}
