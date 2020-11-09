/*  STLover - Simple STL Viewer
 *  Copyright (C) 2020 Gerasim Troeglazov <3dEyes@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.

 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.

 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "STLApp.h"
#include "STLWindow.h"

STLoverApplication::STLoverApplication():BApplication(APP_SIGNATURE)
{
	InstallMimeType();

	BRect windowRect(100, 100, 100 + 640, 100 + 480);
	stlWindow = new STLWindow(windowRect);
}

void
STLoverApplication::RefsReceived(BMessage* msg)
{
	entry_ref ref;

	if (msg->FindRef("refs", 0, &ref) == B_OK) {
		BEntry entry = BEntry(&ref);
		BPath path;
		entry.GetPath(&path);
		stlWindow->OpenFile(path.Path());
	}
}

void
STLoverApplication::InstallMimeType(void)
{
	BMimeType mime(STL_SIGNATURE);
	status_t ret = mime.InitCheck();
	if (ret != B_OK)
		return;

	ret = mime.Install();
	if (ret != B_OK && ret != B_FILE_EXISTS)
		return;

	mime.SetShortDescription("STL file");
	mime.SetLongDescription("A file format native to the STereoLithography CAD software");

	BMessage message('extn');
	message.AddString("extensions", "stl");
	mime.SetFileExtensions(&message);

	BResources* resources = AppResources();
	if (resources != NULL) {
		size_t size;
		const void* iconData = resources->LoadResource('VICN', "BEOS:" STL_SIGNATURE,
			&size);
		if (iconData != NULL && size > 0) {
			mime.SetIcon(reinterpret_cast<const uint8*>(iconData), size);
		}
	}
}
