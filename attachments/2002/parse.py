# TODO: romio, na, remove from mpi.h


import os, sys, re


def insert_wa(wa, lines):
    for i in xrange(len(lines)):
        if "HAVE_PRAGMA_WEAK" in lines[i]:
            j = i+1
            while "#endif" not in lines[j]: j += 1
            lines.insert(j, "#elif defined(HAVE_WEAK_ATTRIBUTE)\n")
            lines.insert(j+1, wa)
            return
    print "Warning", wa, "not successfully patched"
    print lines
    sys.exit(1)

def check_exists(func, path, wa):
    found = False
    f = open(path, "r+")
    ext_func = func + "("
    lines = f.readlines()
    for line in lines:
        if ext_func in line:
            insert_wa(wa, lines)
            f.seek(0)
            f.writelines(lines)
            f.truncate()
            found = True
            break
    f.close()
    return found

def walk_dir(d, exclude, func, wa):
    for dirpath, dirnames, filenames in os.walk(d):
        if exclude == "" or exclude not in dirpath:
            for filename in filenames:
                if filename[-1] == "c" and check_exists(func, dirpath + "/" + filename, wa):
                    return True
    return False


def parse(d, header, exclude, optdir, unoptdirs):
    fh = open(d + "/include/" + header)

    line = fh.readline()
    while("/* Begin Prototypes */" not in line): line = fh.readline()

    na = []
    ctr = 0
    while("/* End Prototypes */" not in line):
        sp = line.split()
        if len(sp) and sp[0] in ("int", "double"):
            while ";" not in line: line += fh.readline()
            if "MPICH_ATTR_WEAK_ALIAS" in line:
                idx_i = line.index("_")
                idx_f = line.index("(")
                filename = line[idx_i+1:idx_f].lower() + ".c"
                func = line[:idx_f]
                wa = line.replace("MPICH_ATTR_WEAK_ALIAS", "__attribute__((weak,alias")
                wa = wa.replace(";", "));")
                idxtt_i = wa.find(" MPICH_ATTR_POINTER_WITH_TYPE_TAG(")
                while idxtt_i != -1:
                    idxtt_f = wa.index(")", idxtt_i)
                    wa = wa[:idxtt_i] + wa[idxtt_f+1:]
                    idxtt_i = wa.find(" MPICH_ATTR_POINTER_WITH_TYPE_TAG(")
                wa = re.sub(" +\n", "", wa)

                found = False
                if optdir != "":
                    for dirpath, dirnames, filenames in os.walk(d + "/" + optdir):
                        if (exclude == "" or exclude not in dirpath) and filename in filenames:
                            found = check_exists(func, dirpath + "/" + filename, wa)
                            if found: break

                if not found:
                    found = walk_dir(d + "/" + optdir, exclude, func, wa)

                    if not found:
                        for ud in unoptdirs:
                            found = walk_dir(d + "/" + ud, exclude, func, wa)
                            if found: break

                        if not found: na.append(func)

                if found: ctr += 1
  
        line = fh.readline()

    print ctr, "modifications"

    if len(na):
        print "NOT FOUND:"
        for f in na:
            print " ", f

    fh.close()


parse(d="src", header="mpi.h.in", exclude="romio", optdir="mpi", unoptdirs=("binding", "mpi_t"))
parse(d="src/mpi/romio", header="mpio.h.in", exclude="", optdir="", unoptdirs=("mpi-io"))
# this leaves out MPI_File_f2c and MPI_File_c2f but it's not worth hacking more the script, and doing it by hand is easier and cleaner
