#include "ServiceLocator.h"

Audio* ServiceLocator::service_{ nullptr };
NullAudio ServiceLocator::nullService_{};