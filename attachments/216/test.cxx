#include <mpi.h>
#include <iostream.h>

/*
 Correct output should be:
 
 Get exeption as expected with error: 
 Expected due to setting MPI::ERRORS_RETURN

*/

/* Argument passed is set to 1 if error handler set to MPI::ERRORS_THROW_EXCEPTIONS */

void error_call(int passed)
{
    int i;

    try {
    /* Do errorness call with negtive count and negative tag */
	MPI::COMM_WORLD.Send(&i, -1, MPI::INT, 0, -10);
	if (passed == 1) {
	    std::cout << "ERROR!! Unexpected go without exception" << std::endl;
	} else {
	    std::cout << "Expected due to setting MPI::ERRORS_RETURN" << std::endl;
	}
	
    }
    catch (MPI::Exception& e) {
	if(passed == 1) {
	/* We expect exception if error handler was set to MPI::ERRORS_THROW_EXCEPTIONS */
	    std::cout << "Get exeption as expected with error: " << std::endl;
	} else {
	    std::cout << "ERROR!! Get unexpected exeption " << std::endl;
	}
    }
    return;
}

int main(int argc, char* argv[])
{
    
    MPI::Init();
    
    MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_THROW_EXCEPTIONS);
    error_call(1);

    MPI::COMM_WORLD.Set_errhandler(MPI::ERRORS_RETURN);
    error_call(0);
    
    MPI::Finalize();
    
    return 0;
}
