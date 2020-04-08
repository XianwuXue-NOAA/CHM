//
// Canadian Hydrological Model - The Canadian Hydrological Model (CHM) is a novel
// modular unstructured mesh based approach for hydrological modelling
// Copyright (C) 2018 Christopher Marsh
//
// This file is part of Canadian Hydrological Model.
//
// Canadian Hydrological Model is free software: you can redistribute it and/or
// modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Canadian Hydrological Model is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Canadian Hydrological Model.  If not, see
// <http://www.gnu.org/licenses/>.
//

#pragma once

#include "logger.hpp"
#include "triangulation.hpp"
#include "module_base.hpp"

#include <cstdlib>
#include <string>
#include <utility> //for pair
#include <vector>
#include <cmath>
#include <armadillo>
#define _USE_MATH_DEFINES
#include <math.h>
#include "tbb/parallel_sort.h"
#include "tbb/task_scheduler_init.h"
struct module_shadow_face_info :  face_info
{
    module_shadow_face_info()
    {
        shadow = 0;
        z_prime = 0;
    }

    int shadow;
    double z_prime;
};

struct vertex_data : vertex_info
{

    Point_3 prj_vertex;
    Point_3 org_vertex;
};

/**
* \addtogroup modules
* @{
* \class Marsh_shading_iswr
* \brief Computes self and horizon shading
*
* Computes the self- and horizon-shadows for a basin. Numerically intensive.
*
* Depends:
* - None
*
* Provides:
* - Value that provides a metric for triangle 'nearness' to the sun "z_prime" [-]
* - Binary shadow value "shadowed" [1 (true) / 2 (false)]
*
*
* Reference:
* > Marsh, C.B., J.W. Pomeroy, and R.J. Spiteri. “Implications of Mountain Shading on Calculating Energy for Snowmelt Using Unstructured Triangular Meshes.” Hydrological Processes 26, no. 12 (June 15, 2012): 1767–78. doi:10.1002/hyp.9329.
*/
class Marsh_shading_iswr final : public module_base
{
REGISTER_MODULE_HPP(Marsh_shading_iswr);
    public:
        Marsh_shading_iswr(config_file cfg);
        ~Marsh_shading_iswr();
        virtual void run(mesh& domain);

    int x_AABB;
    int y_AABB;
};

/**
@}
*/
