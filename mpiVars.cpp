#include "mpiVars.h"
#include "solve.h"


MPI_Datatype mpi_flux_data_type;

void initializeTypes() {
    const int nitems=2;
    int          blocklengths[2] = {2,1};
    MPI_Datatype types[2] = {MPI_INT, MPI_DOUBLE};
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(FluxData, globalId);
    offsets[1] = offsetof(FluxData, flux);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_flux_data_type);
    MPI_Type_commit(&mpi_flux_data_type);
}
