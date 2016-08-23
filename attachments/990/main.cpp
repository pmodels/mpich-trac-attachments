#include <mpi.h>
#include <stdio.h>

#pragma comment(lib, "cxx.lib")
#pragma comment(lib, "mpi.lib")

namespace mpi {
	int gl_my_node;
};

int main() {
	char *buf = new char[MPI::MAX_PROCESSOR_NAME];
	int version, subversion, len;

	printf("initialising MPI subsystem...\n");
	fflush(stdout);

	MPI::Init();
	printf("MPI initialised!\n");

	mpi::gl_my_node = MPI::COMM_WORLD.Get_rank();
	MPI::Get_version(version, subversion);
	MPI::Get_processor_name(buf, len);

	printf("MPI version %d.%d initialised on node '%s'\n", version, subversion, buf);
	printf("current node has id %d of %d\n", mpi::gl_my_node, MPI::COMM_WORLD.Get_size() - 1);

	fflush(stdout);
	MPI::Finalize();
	return 0;
}
