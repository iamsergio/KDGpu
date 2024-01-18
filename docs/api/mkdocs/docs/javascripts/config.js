/*
  This file is part of KDGpu.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

window.MathJax = {
    tex : {
        inlineMath : [ [ "\\(", "\\)" ] ],
        displayMath : [ [ "\\[", "\\]" ] ],
        processEscapes : true,
        processEnvironments : true
    },
    options : {
        ignoreHtmlClass : ".*|",
        processHtmlClass : "arithmatex"
    }
};
