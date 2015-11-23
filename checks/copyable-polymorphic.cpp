/*
   This file is part of the clang-lazy static checker.

  Copyright (C) 2015 Sergio Martins <smartins@kde.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

  As a special exception, permission is given to link this program
  with any edition of Qt, and distribute the resulting executable,
  without including the source code for Qt in the source distribution.
*/

#include "copyable-polymorphic.h"
#include "Utils.h"
#include "checkmanager.h"
#include "StringUtils.h"

#include <clang/AST/AST.h>
#include <clang/Lex/Lexer.h>

using namespace clang;
using namespace std;


CopyablePolymorphic::CopyablePolymorphic(const std::string &name)
    : CheckBase(name)
{
}

void CopyablePolymorphic::VisitDecl(clang::Decl *decl)
{
    CXXRecordDecl *record = dyn_cast<CXXRecordDecl>(decl);
    if (!record || !record->hasDefinition() || record->getDefinition() != record || !record->isPolymorphic())
        return;

    CXXConstructorDecl *copyCtor = Utils::copyCtor(record);
    CXXMethodDecl *copyAssign = Utils::copyAssign(record);

    const bool hasCallableCopyCtor = copyCtor && !copyCtor->isDeleted() && copyCtor->getAccess() != clang::AS_private;
    const bool hasCallableCopyAssign = copyAssign && !copyAssign->isDeleted() && copyAssign->getAccess() != clang::AS_private;

    if (!hasCallableCopyCtor && !hasCallableCopyAssign)
        return;


    emitWarning(record->getLocStart(), "Polymorphic class is copyable. Potential slicing.");
}


REGISTER_CHECK_WITH_FLAGS("copyable-polymorphic", CopyablePolymorphic, CheckLevel3)