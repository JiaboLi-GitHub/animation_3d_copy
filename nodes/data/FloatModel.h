#pragma once

#include <QObject>
#include <QtNodes/NodeDelegateModel>
#include "../NodeData.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;
class FloatModel  : public NodeDelegateModel
{
	Q_OBJECT

public:
	FloatModel();
    virtual ~FloatModel() override;

    bool captionVisible() const override;
    QString caption() const override;
    bool portCaptionVisible(PortType portType, PortIndex portIndex) const override;
    QString portCaption(PortType portType, PortIndex portIndex) const override;
    QString name() const override;
    QJsonObject save() const override;
    void load(QJsonObject const& modelJson) override;
    unsigned int nPorts(PortType portType) const override;
    NodeDataType dataType(PortType portType, PortIndex portIndex) const override;
    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex const portIndex) override;
    std::shared_ptr<NodeData> outData(PortIndex const port) override;
    QWidget* embeddedWidget() override;

private slots:
    void onTextEdited(QString const& string);

private:
    std::shared_ptr<FloatData> m_data;
    QLineEdit* m_lineEdit;
};
