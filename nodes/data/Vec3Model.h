#pragma once

#include <QObject>
#include <QtNodes/NodeDelegateModel>

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class QLineEdit;
class Vec3Data;
class Vec3Model  : public NodeDelegateModel
{
	Q_OBJECT

public:
	Vec3Model();
	~Vec3Model() override;

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
    void slotTextEdited(QString const& string);

private:
    std::shared_ptr<Vec3Data> m_data;
    QWidget* m_widget;
    QLineEdit* m_lineEditX;
    QLineEdit* m_lineEditY;
    QLineEdit* m_lineEditZ;
};
