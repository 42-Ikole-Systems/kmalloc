# ✔️ kmalloc
Dynamically allocating memory! fast!
My own malloc implementation!

# 📥 Installation
```bash
# Clone the repository
$ git clone --recurse-submodules https://github.com/42-Ikole-Systems/kmalloc

# Build the project
$ make -C kmalloc

# Run the program!
$ ./kmalloc/kmalloc
```

# 🧠 Terminology
- **Arena:** a self-contained indipendent memory allocator.
- **Chunk:** 1024 contiguous pages (4MiB).
- **Page run:** 1+ contiguous pages within a chunk.
- **Region:** contiguous bytes that can be used for an allocation.
- **Zone:** pre-allocated contiguous chunk(s) for small or large allocations.
- **Slab:** a region that can accommodate a specific amount of objects of the same size. 

# ℹ️ Sources and References
- [jemalloc paper](https://people.freebsd.org/~jasone/jemalloc/bsdcan2006/jemalloc.pdf)
- [jemalloc talk](https://www.youtube.com/watch?v=RcWp5vwGlYU&list=PLn0nrSd4xjjZoaFwsTnmS1UFj3ob7gf7s)
- [jemmaloc manual](https://jemalloc.net/jemalloc.3.html)

# 🧡 Project tempalate
This project was created from an awesome c project template created with ❤️ by [K1ngmar](https://github.com/K1ngmar).  
Check out the template [here!](https://github.com/K1ngmar/C-project-template)
