#include "UUID.hpp"

#include <random>

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<long long> s_uniformDistribution;

UUID::UUID()
    :
    m_UUID(s_uniformDistribution(s_engine)) { }

UUID::UUID(long long uuid)
    :
    m_UUID(uuid) { }
