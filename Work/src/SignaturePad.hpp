/*
 * Copyright (c) 2012-2014 Research In Motion Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SIGNATUREPAD_HPP_
#define SIGNATUREPAD_HPP_

#include "views/cascades/ViewControl.hpp"
#include "SketchPad.hpp"

using namespace bb::cascades;

namespace views {
	namespace cascades {

class SignaturePad: public ViewControl {

    Q_OBJECT

	//Q_PROPERTY(bool enabled READ enabled WRITE setEnabled)
	//Q_PROPERTY(bool visible READ visible WRITE setVisible)

public:
    SignaturePad(Container * parent = 0);
    virtual ~SignaturePad();

Q_SIGNALS:
    void saved();

public Q_SLOTS:
	void saveImage(const QString& filename);
	void onCaptured();

private:
	QString _saveFilename;

	SketchPad *_signPad;
};

	} /* end namespace cascades */

} /* end namespace views */

#endif /* SIGNATUREPAD_HPP_ */
