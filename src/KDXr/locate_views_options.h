/*
  This file is part of KDXr.

  SPDX-FileCopyrightText: 2024 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

#include <KDXr/kdxr_core.h>
#include <KDGpu/handle.h>

namespace KDXr {

struct ReferenceSpace_t;

struct LocateViewsOptions {
    Time displayTime{ 0 };
    KDGpu::Handle<ReferenceSpace_t> referenceSpace;
};

} // namespace KDXr
