cmake_minimum_required(VERSION 2.8)


add_library(etcd etcd_cli.h etcd_cli.cpp service_provider.h service_provider.cpp )

link_directories(
	${GEN_LIB_DIR}
)
