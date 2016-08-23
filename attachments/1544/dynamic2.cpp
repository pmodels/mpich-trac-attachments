#include <iostream>
#include "mpi.h"

using namespace std;

const int BUFSIZE = 1000;
const int NWORKER = 10;
const int NPASS = 10;

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  MPI_Comm parent;
  MPI_Comm_get_parent(&parent);

  // Master
  if (parent == MPI_COMM_NULL) {
    for (int i = 0; i < NPASS; i++) {
      cout << "pass " << i << " =============" << endl;
      MPI_Comm intercom = MPI_COMM_NULL;
      cout << "spawn " << NWORKER << endl;
      MPI_Comm_spawn(argv[0], MPI_ARGV_NULL, NWORKER, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercom, MPI_ERRCODES_IGNORE);
      for (int worker = 0; worker < NWORKER; worker++) {
	cout << "stop " << worker << endl;
	char buf[BUFSIZE];
	MPI_Send(buf, 0, MPI_CHAR, worker, 0, intercom);
      }
      cout << "disconnnect" << endl;
      MPI_Comm_disconnect(&intercom);
      intercom = MPI_COMM_NULL;
    }
  }

  // Worker
  if (parent != MPI_COMM_NULL) {
    char buf[BUFSIZE];
    MPI_Status status;
    MPI_Recv(buf, BUFSIZE, MPI_CHAR, 0, MPI_ANY_TAG, parent, &status);
    MPI_Comm_disconnect(&parent);
  }

  MPI_Finalize();

  return 0;
}
